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
#include <cstdint>
#include <stdexcept>
#include <memory>
#include <cstdlib>

// Template independent implementation of the MultiModelFitter
class MultiModelFitter_impl {
public:
    typedef int32_t label_type;
    typedef size_t sampleid_type;
protected:
    // Direct call to run algorithm
    std::vector<label_type> fit_impl() const;

    // Virtual callbacks for evaluation steps that require the
    // knowledge of the templates
    virtual sampleid_type get_sample_count() const = 0;
    virtual label_type get_hypothesis_count() const = 0;
    virtual void debug_output(std::vector<label_type> const &) = 0;
    virtual std::shared_ptr<std::vector<std::array<sampleid_type,2>>>
        getNeighbourhood() const = 0; 
    virtual double getResidual(sampleid_type sample, label_type label) const = 0;
    virtual double getNoiseLevel() const = 0;

private:
    // other internal algorithm functions, that shouldn't be visible in child

};

/*
 * Template parameters:
 * - C: the config class
 */
template<class C>
class MultiModelFitter : private MultiModelFitter_impl {
public:
    typedef std::shared_ptr<std::vector<typename C::sample_type>> sample_ptr_type;
    typedef std::shared_ptr<std::vector<typename C::hypothesis_type>> hypothesis_ptr_type;

public:
    // Sets the samples
    void set_samples(std::vector<typename C::sample_type> const & points);
    // Sets the hypotheses
    void set_hypotheses(std::vector<typename C::hypothesis_type> const & hypotheses);
    std::vector<label_type> fit(C& config);
    // Removes the samples, frees memory
    void clear_samples();
    // Removes the hypotheses, frees memory
    void clear_hypotheses();

private:
    // Internal variable, holds samples
    sample_ptr_type samples;
    // Internal variable, holds hypotheses
    hypothesis_ptr_type hypotheses;
    // Handle to the config object
    C* config;
private:
    // Callback functions. This is needed because the
    // actual implementation is free of templates.
    sampleid_type get_sample_count() const;
    label_type get_hypothesis_count() const;
    std::shared_ptr<std::vector<std::array<sampleid_type,2>>>
        getNeighbourhood() const; 
    double getResidual(sampleid_type sample, label_type label) const;
    double getNoiseLevel() const;
    void debug_output(std::vector<label_type> const &);
};

template<class C>
inline void MultiModelFitter<C>::set_samples(std::vector<typename C::sample_type> const & samples)
{
    this->samples = std::make_shared<std::vector<typename C::sample_type>>();
    this->samples->assign(samples.begin(), samples.end());
}

template<class C>
inline void MultiModelFitter<C>::set_hypotheses(std::vector<typename C::hypothesis_type> const & hypotheses)
{
    MultiModelFitter_impl::label_type hypothesis_size = static_cast<MultiModelFitter_impl::label_type>(hypotheses.size());
    if (hypothesis_size != hypotheses.size()) {
        throw std::runtime_error("Too many hypotheses!");
    }
    this->hypotheses = std::make_shared<std::vector<typename C::hypothesis_type>>();
    this->hypotheses->assign(hypotheses.begin(), hypotheses.end());
}

template<class C>
inline std::vector<typename MultiModelFitter_impl::label_type> MultiModelFitter<C>::fit(C& config)
{
    this->config = &config;
    auto result = MultiModelFitter_impl::fit_impl();
    this->config = NULL;
    return result;
}

template<class C>
inline void MultiModelFitter<C>::clear_samples()
{
    this->samples = NULL;
}

template<class C>
inline void MultiModelFitter<C>::clear_hypotheses()
{
    this->hypotheses = NULL;
}

template<class C>
inline MultiModelFitter_impl::sampleid_type MultiModelFitter<C>::get_sample_count() const
{
    return static_cast<MultiModelFitter_impl::sampleid_type>(this->samples->size());
}

template<class C>
inline MultiModelFitter_impl::label_type MultiModelFitter<C>::get_hypothesis_count() const
{
    return static_cast<MultiModelFitter_impl::label_type>(this->hypotheses->size());
}

template<class C>
inline void MultiModelFitter<C>::debug_output(std::vector<label_type> const &labels)
{
    config->debug_output(labels);
}

template<class C>
inline std::shared_ptr<std::vector<std::array<MultiModelFitter_impl::sampleid_type,2>>>
MultiModelFitter<C>::getNeighbourhood() const
{
    return this->config->computeNeighbourhood(*samples); 
}

template<class C>
inline double
MultiModelFitter<C>::getResidual(MultiModelFitter_impl::sampleid_type sample,
                                 MultiModelFitter_impl::label_type label) const
{
    return this->config->computeResidual( samples->at(sample),
                                          hypotheses->at(label) );
}

template<class C>
inline double
MultiModelFitter<C>::getNoiseLevel() const
{
    return this->config->getNoiseLevel();
}
