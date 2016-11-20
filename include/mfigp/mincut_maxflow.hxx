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

namespace mfigp {

template<typename C>
class MinCut_MaxFlow {

    public:
        typedef typename C::computation_type   computation_type;
        typedef typename C::label_type          label_type;
        typedef typename C::sampleid_type      sampleid_type;

    public:
        static void run(
            sampleid_type sample_count,
            label_type hypothesis_count,
            sampleid_type sample_stride,
            label_type label_stride,
            label_type alpha_label,
            std::vector<label_type> const & labeling,
            std::vector<label_type> & new_labeling,
            std::vector<std::array<sampleid_type, 2>> const & neighbourhood,
            computation_type smoothing_penalty,
            std::vector<computation_type> const & fitting_penalties,
            std::vector<computation_type> const & hypothesis_penalties,
            std::vector<computation_type> const & hypothesis_interaction_penalties
        );

};

template<typename C>
inline void MinCut_MaxFlow<C>::run(
    sampleid_type sample_count,
    label_type hypothesis_count,
    sampleid_type sample_stride,
    label_type label_stride,
    label_type alpha_label,
    std::vector<label_type> const & labeling,
    std::vector<label_type> & new_labeling,
    std::vector<std::array<sampleid_type, 2>> const & neighbourhood,
    computation_type smoothing_penalty,
    std::vector<computation_type> const & fitting_penalties,
    std::vector<computation_type> const & hypothesis_penalties,
    std::vector<computation_type> const & hypothesis_interaction_penalties
){

    new_labeling[1] = alpha_label;

}



}//namespace mfigp
