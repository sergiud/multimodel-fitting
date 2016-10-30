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

// Superclass needed, because template classes cannot get compiled to a library
class MultiModelFitter_impl {
protected:
	// Direct call to run algorithm
	std::vector<uint32_t> fit_impl() const;

	// Virtual callbacks for evaluation steps that require the
	// knowledge of the templates
	virtual size_t get_sample_count() = 0;
	virtual uint32_t get_hypotheses_count() = 0;
};

/*
 * Template parameters:
 * - C: the config class
 */
template<class C>
class MultiModelFitter : private MultiModelFitter_impl {

public:
	// The actual functionality of this class
	void set_samples(const &std::vector<C::sample_type> points);
	void set_hypotheses(const &std::vector<C::hypothesis_type> hypotheses);
	std::vector<uint32_t> fit() const;
	void clear_samples();
	void clear_hypotheses();

private:
	// Internal variables
	std::vector<C::sample_type> samples;
	std::vector<C::hypothesis_type> hypotheses;

private:
	// Callback functions. This is needed because the
	// actual implementation is free of templates.
	size_t get_sample_count();
	uint32_t get_hypotheses_count();
};

template<class C>
inline void MultiModelFitter<C>::set_samples(const &std::vector<C::sample_type> samples)
{
	this->samples = samples;
}

template<class C>
inline void MultiModelFitter<C>::set_hypotheses(const &std::vector<C::hypothesis_type> hypothesis)
{
	uint32_t hypothesis_size = static_cast<uint32_t>(hypotheses.size());
	if (hypothesis_size != hypotheses.size()) {
		throw std::runtime_error("Too many hypotheses!")
	}
	this->hypotheses = hypotheses;
}

template<class C>
inline std::vector<uint32_t> MultiModelFitter<C>::fit() const
{
	return MultiModelFitter_impl::fit();
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
inline size_t MultiModelFitter<C>::get_sample_count()
{
	return this->samples.size();
}

template<class C>
inline uint32_t MultiModelFitter<C>::get_hypotheses_count()
{
	return static_cast<uint32_t>(this->hypotheses.size());
}