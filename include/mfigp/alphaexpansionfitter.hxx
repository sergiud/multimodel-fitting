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
#include <set>
#include <functional>
#include <cassert>

#include "mincut_maxflow.hxx"

namespace mfigp {

template<typename C>
class AlphaExpansionFitter {

    public:
        typedef typename C::computation_type    computation_type;
        typedef typename C::label_type          label_type;
        typedef typename C::sampleid_type       sampleid_type;
        typedef typename std::make_unsigned<label_type>::type
                                                internal_label_type;

    public:

        static std::vector<label_type> fit(
            C & config,
            size_t sample_count,
            size_t hypothesis_count,
            size_t label_stride,
            std::vector< std::array<sampleid_type,2> > const & neighbourhood,
            computation_type smoothing_penalty,
            std::vector<computation_type> const & fitting_penalties,
            std::vector<computation_type> const & hypothesis_penalties,
            std::vector<computation_type> const & hypothesis_interaction_penalties
        );

    private:
        static computation_type compute_value(
            size_t sample_count,
            size_t label_stride,
            std::vector<internal_label_type> const & labeling,
            std::vector<std::array<sampleid_type, 2>> const & neighbourhood,
            computation_type smoothing_penalty,
            std::vector<computation_type> const & fitting_penalties,
            std::vector<computation_type> const & hypothesis_penalties,
            std::vector<computation_type> const & hypothesis_interaction_penalties
        );

};

template<typename C>
inline typename C::computation_type
AlphaExpansionFitter<C>::compute_value(
    size_t sample_count,
    size_t label_stride,
    std::vector<internal_label_type> const & labeling,
    std::vector< std::array<sampleid_type,2> > const & neighbourhood,
    computation_type smoothing_penalty,
    std::vector<computation_type> const & fitting_penalties,
    std::vector<computation_type> const & hypothesis_penalties,
    std::vector<computation_type> const & hypothesis_interaction_penalties )
{
    computation_type result = 0.0;

    // Fitting penalty
    for(sampleid_type sample_id = 0; sample_id < sample_count; sample_id++){
        internal_label_type label = labeling[sample_id];
        result += fitting_penalties[sample_id * label_stride + label];
    }

    // Smoothing penalty
    for(auto const & neighbour_pair : neighbourhood){
        internal_label_type label0 = labeling[neighbour_pair[0]];
        internal_label_type label1 = labeling[neighbour_pair[1]];
        if(label0 != label1)
            result += smoothing_penalty;
    }

    // Hypothesis Penalties
    std::set<internal_label_type> active_labels( labeling.begin(),
                                                 labeling.end() );

    for(auto const & label : active_labels){
        result += hypothesis_penalties[label];
    }

    // Hypothesis Interaction Penalties
    for(auto const & label1 : active_labels){
        for(auto const & label2 : active_labels){
            if(label1 >= label2) continue;
            result += hypothesis_interaction_penalties[label1* label_stride + label2];
        }
    }

    return result;
}

template<typename C>
inline std::vector<typename C::label_type>
AlphaExpansionFitter<C>::fit(
    C & config,
    size_t sample_count,
    size_t hypothesis_count,
    size_t label_stride,
    std::vector< std::array<sampleid_type,2> > const & neighbourhood,
    computation_type smoothing_penalty,
    std::vector<computation_type> const & fitting_penalties,
    std::vector<computation_type> const & hypothesis_penalties,
    std::vector<computation_type> const & hypothesis_interaction_penalties )
{

    // Create initial labeling
    std::vector<internal_label_type> labeling_array_1(sample_count);
    std::vector<internal_label_type> labeling_array_2(sample_count);

    std::vector<internal_label_type> *labeling = &labeling_array_1;
    std::vector<internal_label_type> *new_labeling = &labeling_array_2;
    for(sampleid_type i = 0; i < labeling->size(); i++){
        (*labeling)[i] = 0;
    }

    bool changed;
    bool outlier_check_needed = false;
    computation_type current_value = compute_value(
                                          sample_count,
                                          label_stride,
                                          *labeling,
                                          neighbourhood,
                                          smoothing_penalty,
                                          fitting_penalties,
                                          hypothesis_penalties,
                                          hypothesis_interaction_penalties );
    config.debug_output(*labeling, current_value);
    do {
        changed = false;

        for(internal_label_type alpha_label = 1; alpha_label < hypothesis_count; alpha_label++)
        {
            //std::cout << "alpha: " << alpha_label << std::endl;

            // Do the actual graph cut
            MinCut_MaxFlow<C, internal_label_type>::run(
                sample_count,
                label_stride,
                alpha_label,
                *labeling,
                *new_labeling,
                neighbourhood,
                smoothing_penalty,
                fitting_penalties,
                hypothesis_penalties,
                hypothesis_interaction_penalties
            );

            // Compute new value
            computation_type new_value = compute_value(
                sample_count,
                label_stride,
                *new_labeling,
                neighbourhood,
                smoothing_penalty,
                fitting_penalties,
                hypothesis_penalties,
                hypothesis_interaction_penalties
            );

            // Swap if new optimum found
            if(new_value < current_value){
                /*std::cout << "alpha: " << alpha_label << std::endl;
                for(size_t i = 0; i < sample_count; i++){
                    if(labeling->at(i) != new_labeling->at(i)){
                        std::cout << i << ": " << labeling->at(i) << " -> " << new_labeling->at(i) << std::endl;
                    }
                }*/
                std::swap(labeling, new_labeling);
                current_value = new_value;
                config.debug_output(*labeling, current_value);
                changed = true;
                outlier_check_needed = true;
            }

            // Check for outliers every 20 labels
            if(outlier_check_needed){
                outlier_check_needed = false;

                // Do the actual graph cut
                MinCut_MaxFlow<C, internal_label_type>::run(
                    sample_count,
                    label_stride,
                    0,
                    *labeling,
                    *new_labeling,
                    neighbourhood,
                    smoothing_penalty,
                    fitting_penalties,
                    hypothesis_penalties,
                    hypothesis_interaction_penalties
                );

                // Compute new value
                new_value = compute_value(
                    sample_count,
                    label_stride,
                    *new_labeling,
                    neighbourhood,
                    smoothing_penalty,
                    fitting_penalties,
                    hypothesis_penalties,
                    hypothesis_interaction_penalties
                );

                // Swap if new optimum found
                if(new_value < current_value){
                    /*std::cout << "alpha: " << 0 << std::endl;
                    for(size_t i = 0; i < sample_count; i++){
                        if(labeling->at(i) != new_labeling->at(i)){
                            std::cout << i << ": " << labeling->at(i) << " -> " << new_labeling->at(i) << std::endl;
                        }
                    }*/
                    std::swap(labeling, new_labeling);
                    current_value = new_value;
                    config.debug_output(*labeling, current_value);
                    changed = true;
                }
            }
        }

    } while (changed);

    assert(labeling == &labeling_array_1 || labeling == &labeling_array_2);
    std::vector<label_type> result_labels(sample_count);

    std::transform(labeling->begin(), labeling->end(), result_labels.begin(),
                   [](internal_label_type x){return label_type(x)-1;});

    return result_labels;
}

}//namespace mfigp
