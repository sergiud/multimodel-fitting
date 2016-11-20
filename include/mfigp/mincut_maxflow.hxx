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

#include "ext/block.h"
#include "ext/graph.h"
#include "ext/graph.tpp"

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
    typedef ext::maxflow::Graph< computation_type,
                                 computation_type,
                                 computation_type > GraphType;

    // Create graph
    GraphType g(2*sample_count, 4*sample_count);

    // compute active labels
    std::set<label_type> active_labels(labeling.begin(), labeling.end());

    // add all sample nodes
    g.add_node(sample_count);

    // add smooting costs
    std::vector<computation_type> extra_smoothing_costs(sample_count);
    for(sampleid_type i = 0; i<extra_smoothing_costs.size(); i++){
        extra_smoothing_costs[i] = 0;
    }
    for(auto const & neighbourPair : neighbourhood){
        sampleid_type sample0 = neighbourPair[0];
        sampleid_type sample1 = neighbourPair[1];

        label_type label0 = labeling[sample0];
        label_type label1 = labeling[sample1];

        if(label0 == alpha_label){
            if(label1 == alpha_label){
                continue;
            }
            extra_smoothing_costs[sample1] += smoothing_penalty;
            continue;
        }

        if(label1 == alpha_label){
            extra_smoothing_costs[sample0] += smoothing_penalty;
            continue;
        }

        if(label0 == label1){
            g.add_edge(sample0, sample1, smoothing_penalty, smoothing_penalty);
            continue;
        }

        auto n0 = g.add_node();

        g.add_tweights(n0, 0, smoothing_penalty);
        g.add_edge(sample0, n0, 0, smoothing_penalty);
        g.add_edge(sample1, n0, 0, smoothing_penalty);
    }

    // set up data cost
    for(sampleid_type sampleid = 0; sampleid < sample_count; sampleid++){

        auto const & currentLabel = labeling[sampleid];

        if(currentLabel == alpha_label){
            g.add_tweights( sampleid, 1, 0 );
        } else {
            g.add_tweights( sampleid,
                fitting_penalties[ sampleid*label_stride + alpha_label ],
                fitting_penalties[ sampleid*label_stride + currentLabel ] +
                    extra_smoothing_costs[sampleid] );
        }

    }


    g.maxflow();

    for(sampleid_type i = 0; i < sample_count; i++){
        if(g.what_segment(i) == GraphType::SOURCE){
            new_labeling[i] = labeling[i];
        } else {
            new_labeling[i] = alpha_label;
        }
    }

}



}//namespace mfigp
