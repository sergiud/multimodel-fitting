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

namespace mfigp {

template<typename C>
class AlphaExpansionFitter {

    public:
        typedef typename C::computation_type   computation_type;
        typedef typename C::label_type         label_type;
        typedef typename C::sampleid_type      sampleid_type;

    public:

        static std::vector<label_type> fit(
            label_type hypothesis_count,
            sampleid_type sample_count,
            label_type label_stride,
            sampleid_type sample_stride,
            std::vector< std::array<sampleid_type,2> > neighbourhood,
            computation_type smoothing_penalty,
            std::vector<computation_type> fitting_penalties,
            std::vector<computation_type> hypothesis_penalties,
            std::vector<computation_type> hypothesis_interaction_penalties
        );

};

template<typename C>
inline std::vector<typename C::label_type>
AlphaExpansionFitter<C>::fit(
    typename C::label_type hypothesis_count,
    typename C::sampleid_type sample_count,
    typename C::label_type label_stride,
    typename C::sampleid_type sample_stride,
    std::vector< std::array<typename C::sampleid_type,2> > neighbourhood,
    typename C::computation_type smoothing_penalty,
    std::vector<typename C::computation_type> fitting_penalties,
    std::vector<typename C::computation_type> hypothesis_penalties,
    std::vector<typename C::computation_type> hypothesis_interaction_penalties )
{
    return std::vector<label_type>(sample_count);
}

}
