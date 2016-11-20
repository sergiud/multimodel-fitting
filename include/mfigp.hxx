/*
 * Copyright (C) 2016  Martin Stumpf
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <vector>
#include <array>
#include <algorithm>

#include "mfigp/alphaexpansionfitter.hxx"

namespace mfigp {

/*
 * Template parameters:
 * - C: the config class
 */
template<class C>
class MultiModelFitter {

public:
    typedef typename C::sample_type        sample_type;
    typedef typename C::hypothesis_type    hypothesis_type;
    typedef typename C::computation_type   computation_type;
    typedef typename C::label_type         label_type;
    typedef typename C::sampleid_type      sampleid_type;

public:
    // Sets the samples
    void set_samples(std::vector<sample_type> const & points);
    // Sets the hypotheses
    void set_hypotheses(std::vector<hypothesis_type> const & hypotheses);
    // Runs the algorithm
    std::vector<label_type> fit(C& config);
    // Removes the samples, frees memory
    void clear_samples();
    // Removes the hypotheses, frees memory
    void clear_hypotheses();

private:
    // Internal variable, holds samples
    std::vector<sample_type> samples;
    // Internal variable, holds hypotheses
    std::vector<hypothesis_type> hypotheses;

private:
    // Callback functions. This is needed because the
    // actual implementation is free of templates.
    sampleid_type get_sample_count() const;
    label_type get_hypothesis_count() const;
    std::vector<std::array<sampleid_type,2>>
        getNeighbourhood(C& config) const;
    computation_type getResidual( C& config,
                                  sampleid_type sample,
                                  label_type label) const;
    computation_type getNoiseLevel(C& config) const;
    computation_type getNeighbourhoodWeight(C& config) const;
    computation_type getHighlevelConstraintWeight(C& config) const;
    computation_type getHypothesisCost(C& config, label_type label) const;
    computation_type getHypothesisInteractionCost( C& config, label_type label1,
                                                   label_type label2) const;
};

template<class C>
inline void MultiModelFitter<C>::set_samples(
    std::vector<typename C::sample_type> const & samples)
{
    this->samples.clear();
    this->samples.assign(samples.begin(), samples.end());
}

template<class C>
inline void MultiModelFitter<C>::set_hypotheses(
    std::vector<typename C::hypothesis_type> const & hypotheses)
{
    label_type hypothesis_size = static_cast<label_type>(hypotheses.size());
    if (hypothesis_size != hypotheses.size()) {
        throw std::runtime_error("Too many hypotheses!");
    }
    this->hypotheses.clear();
    this->hypotheses.assign(hypotheses.begin(), hypotheses.end());
}

template<class C>
inline std::vector<typename C::label_type> MultiModelFitter<C>::fit(C& config)
{
    label_type hypothesis_count = get_hypothesis_count();
    sampleid_type sample_count = get_sample_count();
    label_type label_stride = hypothesis_count + 1;
    sampleid_type sample_stride = sample_count;

    // compute fitting penalties
    std::vector<computation_type> fitting_penalties(sample_count * label_stride);
    {
        // get noise level
        computation_type noise_level = config.getNoiseLevel();

        for(sampleid_type sample_id = 0; sample_id < sample_count; sample_id++){

            // Set penalty for outliers
            fitting_penalties[sample_id * label_stride + 0] = 1;

            // Set penalty for inliers
            for(label_type label_id = 0; label_id < hypothesis_count; label_id++){
                computation_type residual = getResidual(config, sample_id, label_id);
                computation_type normalized_residual = residual / noise_level;
                fitting_penalties[sample_id * label_stride + label_id + 1] =
                    normalized_residual * normalized_residual;
            }

        }
    }

    // compute smoothing penalty
    computation_type smoothing_penalty = getNeighbourhoodWeight(config);

    // get highlevel constraint weight
    computation_type highlevel_constraint_weight =
        getHighlevelConstraintWeight(config);

    // compute hypothesis penalties
    std::vector<computation_type> hypothesis_penalties(hypothesis_count + 1);
    {

        // set penalty for outlier label
        hypothesis_penalties[0] = 0;

        // set penalty for inlier labels
        for(label_type label_id = 0; label_id < hypothesis_count; label_id++){
            hypothesis_penalties[label_id+1] = getHypothesisCost(config, label_id)
                                             * highlevel_constraint_weight;
        }

    }

    // compute hypothesis interaction penalties
    std::vector<computation_type> hypothesis_interaction_penalties(
        label_stride*(hypothesis_count + 1) );
    {

        for(label_type label1 = -1; label1 < hypothesis_count; label1++){
            for(label_type label2 = -1; label2 < hypothesis_count; label2++){

                computation_type cost = 0;

                // Equal labels or labels and outliers do not cost anything
                if(label1 >= 0 && label2 >= 0 && label1 != label2){
                    cost = getHypothesisInteractionCost(config, label1, label2);
                }

                hypothesis_interaction_penalties[
                    (label1+1) * label_stride + (label2+1) ] = cost;

            }
        }

    }

    // compute neighbourhood
    std::vector<std::array<sampleid_type, 2>> neighbourhood =
        getNeighbourhood( config );

    // run the actual algorithm
    std::vector<label_type> labels =
        AlphaExpansionFitter<C>::fit(
            config,
            sample_count, hypothesis_count+1, sample_stride, label_stride,
            neighbourhood, smoothing_penalty, fitting_penalties,
            hypothesis_penalties, hypothesis_interaction_penalties
        );

    // transform the results
    std::vector<label_type> transformed_labels(labels.size());
    std::transform(labels.begin(), labels.end(), transformed_labels.begin(),
                   [](label_type x){return x-1;});

    return transformed_labels;

}

template<class C>
inline void MultiModelFitter<C>::clear_samples()
{
    this->samples.clear();
}

template<class C>
inline void MultiModelFitter<C>::clear_hypotheses()
{
    this->hypotheses.clear();
}

template<class C>
inline typename C::sampleid_type
MultiModelFitter<C>::get_sample_count() const
{
    return static_cast<sampleid_type>(samples.size());
}

template<class C>
inline typename C::label_type
MultiModelFitter<C>::get_hypothesis_count() const
{
    return static_cast<label_type>(hypotheses.size());
}

template<class C>
inline std::vector< std::array<typename C::sampleid_type,2> >
MultiModelFitter<C>::getNeighbourhood( C & config ) const
{
    return config.computeNeighbourhood(samples);
}

template<class C>
inline typename C::computation_type
MultiModelFitter<C>::getResidual( C & config,
                                  typename C::sampleid_type sample,
                                  typename C::label_type label ) const
{
    return config.computeResidual( samples[sample],
                                   hypotheses[label] );
}

template<class C>
inline typename C::computation_type
MultiModelFitter<C>::getNoiseLevel( C & config ) const
{
    return config.getNoiseLevel();
}

template<class C>
inline typename C::computation_type
MultiModelFitter<C>::getNeighbourhoodWeight( C & config ) const
{
    return config.getNeighbourhoodWeight();
}

template<class C>
inline typename C::computation_type
MultiModelFitter<C>::getHighlevelConstraintWeight( C & config ) const
{
    return config.getHighlevelPriorsWeight() *
           config.getNumberOfParametersPerHypothesis() *
           log(samples.size()) *
           (1.0f / config.getExpectedNumberOfStructures());
}

template<class C>
inline typename C::computation_type
MultiModelFitter<C>::getHypothesisCost( C & config, typename C::label_type label) const
{
    return config.getHypothesisCost( hypotheses[label] );
}

template<class C>
inline typename C::computation_type
MultiModelFitter<C>::getHypothesisInteractionCost( C & config,
                                                   typename C::label_type label1,
                                                   typename C::label_type label2) const
{
    return config.getHypothesisInteractionCost( hypotheses[label1],
                                                hypotheses[label2] );
}

}