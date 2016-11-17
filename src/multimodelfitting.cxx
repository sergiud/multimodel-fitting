#include "../include/multimodelfitting.hxx"
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

#include <set>
#include <lemon/list_graph.h>
#include <lemon/preflow.h>

#define LARGE_DOUBLE 10000000000.0

double MultiModelFitter_impl::compute_value(
    std::vector<MultiModelFitter_impl::label_type> labeling,
    std::shared_ptr<std::vector<std::array<MultiModelFitter_impl::sampleid_type, 2>>>
        const & neighbourhood,
    double smoothing_penalty,
    std::vector<std::vector<double>> const & fitting_penalties,
    std::vector<double> const & hypothesis_penalties,
    std::vector<std::vector<double>> const & hypothesis_interaction_penalties
) const
{
    double result = 0.0;

    // Fitting penalty
    for(sampleid_type sample_id = 0; sample_id < labeling.size(); sample_id++){
        label_type label = labeling[sample_id];
        result += fitting_penalties[sample_id][label];
    }

    // Smoothing penalty
    for(auto const & neighbour_pair : *neighbourhood){
        label_type label0 = labeling[neighbour_pair[0]];
        label_type label1 = labeling[neighbour_pair[1]];
        if(label0 != label1)
            result += smoothing_penalty;
    }

    // Hypothesis Penalties
    std::set<label_type> active_labels( labeling.begin(),
                                        labeling.end() );

    for(auto const & label : active_labels){
        result += hypothesis_penalties[label];
    }

    // Hypothesis Interaction Penalties
    for(auto const & label1 : active_labels){
        for(auto const & label2 : active_labels){
            if(label1 >= label2) continue;
            result += hypothesis_interaction_penalties[label1][label2];
        }
    }

    return result;
}

std::vector<unsigned char>
MultiModelFitter_impl::graph_cut(
    MultiModelFitter_impl::label_type alpha_label,
    sampleid_type sample_count,
    label_type hypothesis_count,
    std::vector<MultiModelFitter_impl::label_type> const & current_labeling,
    std::shared_ptr<std::vector<std::array<MultiModelFitter_impl::sampleid_type, 2>>>
        const & neighbourhood,
    double smoothing_penalty,
    std::vector<std::vector<double>> const & fitting_penalties,
    std::vector<double> const & hypothesis_penalties,
    std::vector<std::vector<double>> const & hypothesis_interaction_penalties
) const {

    typedef lemon::ListGraph GraphType;
    typedef GraphType::Node NodeType;
    typedef GraphType::EdgeMap<double> CostMapType;

    size_t num_nodes = sample_count;

    ////////////////////////////////////////////////////
    // Initialization. TODO move out of this function, use snapshotting
    // Create graph
    GraphType g;

    // Create node list.
    std::vector<NodeType> nodes;
    nodes.reserve(sample_count);

    // Create ID nodes
    NodeType nodeAlpha = g.addNode();
    NodeType nodeNotAlpha = g.addNode();

    // create sample nodes
    for(sampleid_type sampleid = 0; sampleid < sample_count; sampleid++){
        nodes.push_back(g.addNode());
    }


    ////////////////////////////////////////////////////
    // Build problem specific part of the graph

    // Create lists for auxiliary nodes. just for debugging.
    std::vector<GraphType::Edge> edgesAlpha(sample_count);
    std::vector<GraphType::Edge> edgesNotAlpha(sample_count);

    // TODO remove
    std::vector<NodeType> smoothingAuxNodes;
    std::vector<NodeType> hypothesisAuxNodes;

    // Initialize costmap
    CostMapType cost(g);

    // Set direct connections from sample to node0 and node1
    for(sampleid_type sampleid = 0; sampleid < sample_count; sampleid++){
        NodeType & node = nodes[sampleid];

        auto const & currentLabel = current_labeling[sampleid];

        // ignore labels that are already alpha
        if(currentLabel == alpha_label){
            auto e0 = g.addEdge(node, nodeAlpha);
            cost[e0] = 1;
            edgesAlpha[sampleid] = e0;
            continue;
        }

        auto const & alphaCost = fitting_penalties[sampleid][alpha_label];
        auto const & currentCost = fitting_penalties[sampleid][currentLabel];

        auto e0 = g.addEdge(node, nodeAlpha);
        auto e1 = g.addEdge(node, nodeNotAlpha);
        cost[e0] = alphaCost;
        cost[e1] = currentCost;
        edgesAlpha[sampleid] = e0;
        edgesNotAlpha[sampleid] = e1;
    }

    // Set connections between nodes
    for(auto const & neighbourPair : *neighbourhood){
        sampleid_type sample0 = neighbourPair[0];
        sampleid_type sample1 = neighbourPair[1];

        label_type label0 = current_labeling[sample0];
        label_type label1 = current_labeling[sample1];

        NodeType & node0 = nodes[sample0];
        NodeType & node1 = nodes[sample1];

        if(label0 == alpha_label){
            if(label1 == alpha_label){
                continue;
            } else {
                // TODO beautify
                // swap. now if an alpha label is present, it is always label1.
                label_type tmp = label0;
                label0 = label1;
                label1 = tmp;
                sampleid_type tmp2 = sample0;
                sample0 = sample1;
                sample1 = tmp2;
            }
        }

        if(label1 == alpha_label){
            // specialized algorithm for alpha labels
            cost[edgesNotAlpha[sample0]] += smoothing_penalty;
            continue;
        }


        // normal algorithm, none of the two samples have an alpha label
        if(label0 == label1){
            auto e0 = g.addEdge(node0, node1);
            cost[e0] = smoothing_penalty;
            continue;
        }

        NodeType auxNode = g.addNode();
        smoothingAuxNodes.push_back(auxNode);

        auto e0a = g.addEdge(node0, auxNode);
        auto e1a = g.addEdge(node1, auxNode);
        auto eax = g.addEdge(nodeNotAlpha, auxNode);

        cost[eax] = smoothing_penalty;
        cost[e0a] = smoothing_penalty;
        cost[e1a] = smoothing_penalty;

    }

    {
        std::set<label_type> active_labels( current_labeling.begin(),
                                            current_labeling.end() );

        // Penalty for switch to alpha label
        if(active_labels.count(alpha_label) == 0){
            double c_a = hypothesis_penalties[alpha_label];
            for(auto const & label : active_labels){
                c_a += hypothesis_interaction_penalties[alpha_label][label];
            }
            std::cout << "c_a: " << c_a << std::endl;

            NodeType auxNode = g.addNode();
            hypothesisAuxNodes.push_back(auxNode);
            auto e0 = g.addEdge(nodeAlpha, auxNode);
            cost[e0] = c_a;

            for(sampleid_type sampleid = 0; sampleid < sample_count; sampleid++){
                auto e1 = g.addEdge(nodes[sampleid], auxNode);
                cost[e1] = c_a;
            }
        }

        // Penalty for abandoning other labels
        for(auto const & beta_label : active_labels){
            if(beta_label == alpha_label) continue;

            double c_b = hypothesis_penalties[beta_label]
                         + hypothesis_interaction_penalties[beta_label][alpha_label];

            for(auto const & label : active_labels){
                // TODO check in paper how to deal with existing alpha labels
                if(label == beta_label || label == alpha_label){
                    continue;
                }
                c_b += hypothesis_interaction_penalties[beta_label][label];
            }

            std::cout << "c_" << beta_label << ": " << c_b << std::endl;

            NodeType auxNode = g.addNode();
            hypothesisAuxNodes.push_back(auxNode);
            auto e0 = g.addEdge(nodeNotAlpha, auxNode);
            cost[e0] = c_b;

            for(sampleid_type sampleid = 0; sampleid < sample_count; sampleid++){
                if(current_labeling[sampleid] == beta_label){
                    auto e1 = g.addEdge(nodes[sampleid], auxNode);
                    cost[e1] = c_b;
                }
            }
        }



    }

    ////////////////////////////////////////////////////
    // Run MinCut
    lemon::Preflow<GraphType, CostMapType> flow(g, cost, nodeNotAlpha, nodeAlpha);
    flow.runMinCut();

    ////////////////////////////////////////////////////
    // Read MinCut result
    std::vector<unsigned char> result;
    result.reserve(sample_count);
    for(sampleid_type sampleid = 0; sampleid < sample_count; sampleid++){
        if(flow.minCut(nodes[sampleid])){
            result.push_back(1);
        } else {
            result.push_back(0);
        }
    }

    return result;

}

std::vector<MultiModelFitter_impl::label_type>
MultiModelFitter_impl::fit_impl() const
{
    // IMPORTANT
    // The return type of this function is a vector of label ids.
    // The label can be 0 to hypothesis_count-1 for points that got mapped to
    // hypothesis, and -1 for outliers.
    // INTERNALLY though, all label ids are shifted by one, ranging from 1
    // to hypothesis_count, and 0 for outliers. This is because labels cannot be
    // negative.

    sampleid_type sample_count = get_sample_count();
    label_type hypothesis_count = get_hypothesis_count();

    //////////////////////////////////////////
    // Fetch parameters

    // compute neighbourhood
    auto neighbourhood = getNeighbourhood();

    // Compute smoothing penalty
    double smoothing_penalty = getNeighbourhoodWeight();

    // compute fitting penalties
    std::vector<std::vector<double>> fitting_penalties(sample_count);
    {
        // fetch noise level
        double noiseLevel = getNoiseLevel();
        double noiseLevelSquare = noiseLevel*noiseLevel;

        for(sampleid_type i = 0; i < sample_count; i++){
            std::vector<double> current_fitting_penalties(hypothesis_count + 1);

            current_fitting_penalties[0] = 1.0f;

            for(label_type j = 0; j < hypothesis_count; j++){
                double residual = getResidual(i, j);
                double penalty = (residual*residual)/noiseLevelSquare;
                current_fitting_penalties[j+1] = penalty;
            }

            fitting_penalties[i] = std::move(current_fitting_penalties);
        }
    }

    // compute hypothesis penalties
    std::vector<double> hypothesis_penalties(hypothesis_count + 1);
    {
        double highlevel_constraint_weight = getHighlevelConstraintWeight();
        for(label_type i = 0; i < hypothesis_count + 1; i++){
            hypothesis_penalties[i] = getHypothesisCost(i) * highlevel_constraint_weight;
        }
    }

    // compute hypothesis interaction penalties
    std::vector<std::vector<double>> hypothesis_interaction_penalties(hypothesis_count + 1);
    {
        double highlevel_constraint_weight = getHighlevelConstraintWeight();
        for(label_type i = 0; i < hypothesis_count + 1; i++){
            std::vector<double> current_hypothesis_interaction_penalties(hypothesis_count + 1);
            for(label_type j = 0; j < hypothesis_count + 1; j++){
                current_hypothesis_interaction_penalties[j] =
                    getHypothesisInteractionCost(i,j) * highlevel_constraint_weight;
            }
            hypothesis_interaction_penalties[i] = std::move(current_hypothesis_interaction_penalties);
        }
    }


    ////////////////////////////////////////
    // Solve

    // Alpha Expansion algorithm
    std::vector<label_type> labeling(sample_count);
    for(sampleid_type i = 0; i < labeling.size(); i++){
        labeling[i] = 0;
    }

    bool changed = false;
    double current_value = compute_value( labeling,
                                          neighbourhood,
                                          smoothing_penalty,
                                          fitting_penalties,
                                          hypothesis_penalties,
                                          hypothesis_interaction_penalties );
    debug_output(labeling, current_value);
    do {
        changed = false;

        for(label_type alpha_label = 0; alpha_label < hypothesis_count + 1; alpha_label++)
        {
            std::cout << "-------------" << std::endl;
            std::cout << "alpha: " << alpha_label << std::endl;

            // Run graph cut
            auto alpha_expansion_suggestion = graph_cut( alpha_label,
                                                         sample_count,
                                                         hypothesis_count,
                                                         labeling,
                                                         neighbourhood,
                                                         smoothing_penalty,
                                                         fitting_penalties,
                                                         hypothesis_penalties,
                                                         hypothesis_interaction_penalties );

            // TODO remove
            for(size_t i = sample_count; i < alpha_expansion_suggestion.size(); i++){
                std::cout << "z_" << i << " = "
                          << static_cast<int>(alpha_expansion_suggestion[i])
                          << std::endl;
            }

            // Create potential new labeling
            // TODO optimize
            std::vector<label_type> new_labeling(sample_count);
            for(sampleid_type i = 0; i < sample_count; i++){
                if(alpha_expansion_suggestion[i] == 0){
                    new_labeling[i] = labeling[i];
                } else {
                    new_labeling[i] = alpha_label;
                }
            }

            // Swap depending on wether or not the new labeling is actually better
            double new_value = compute_value( new_labeling,
                                              neighbourhood,
                                              smoothing_penalty,
                                              fitting_penalties,
                                              hypothesis_penalties,
                                              hypothesis_interaction_penalties );

            size_t num_alphalabel = 0;
            for(auto const & sug : alpha_expansion_suggestion){
                if(sug != 0) num_alphalabel ++;
            }

            // TODO remove debug output
            std::cout << "new_value: " << new_value << " - old_value: "
                      << current_value << std::endl;
            std::cout << "num_alphalabel: " << num_alphalabel << std::endl;
            if(new_value < current_value || current_value < 0){
                labeling = std::move(new_labeling);
                current_value = new_value;
                debug_output(labeling, current_value);
                changed = true;
            } else if (num_alphalabel != 0){
                std::cout << "_____________________WARNING______________"<< std::endl;
            }

        }

        // Repeat until convergence
    } while(changed);

    // subtract 1 from every label.
    // output format is: -1 for outlier, label ids starting with 0
    // computation format is: 0 for outlier, label ids starting with 1
    std::transform(labeling.begin(), labeling.end(), labeling.begin(),
                   [](label_type x){return x-1;});
    return labeling;

}
//cmakepackageconfighelpers
//siehe glog
