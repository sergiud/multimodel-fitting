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

/*
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
*/

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
    std::vector<std::vector<double>> const & fitting_penalties,
    std::vector<double> const & hypothesis_penalties,
    std::vector<std::vector<double>> const & hypothesis_interaction_penalties
) const {

    typedef lemon::ListGraph GraphType;

    size_t num_nodes = sample_count;


    //  5--S--10
    //  |     |
    //  A--5--B
    //  |     |
    //  20-D--5

    GraphType g;
    GraphType::EdgeMap<double> cost(g);

    auto nodeA = g.addNode();
    auto nodeB = g.addNode();
    auto nodeS = g.addNode();
    auto nodeD = g.addNode();

    auto eAB = g.addEdge(nodeA, nodeB);
    cost[eAB] = 0.003;
    auto eAS = g.addEdge(nodeA, nodeS);
    cost[eAS] = 0.005;
    auto eAD = g.addEdge(nodeA, nodeD);
    cost[eAD] = 0.020;
    auto eBS = g.addEdge(nodeB, nodeS);
    cost[eBS] = 0.010;
    auto eBD = g.addEdge(nodeB, nodeD);
    cost[eBD] = 0.005;


    lemon::Preflow<GraphType, GraphType::EdgeMap<double>> flow(g, cost, nodeS, nodeD);
    flow.runMinCut();

    std::cout << "A: " << (flow.minCut(nodeA)?"S":"D") << std::endl;
    std::cout << "B: " << (flow.minCut(nodeB)?"S":"D") << std::endl;

    std::vector<unsigned char> labeling(current_labeling.size()+5);
    for(sampleid_type i = 0; i < labeling.size(); i++){
        labeling[i] = 0;
        if(i > labeling.size() - 3) labeling[i] = 1;
    }
    return labeling;

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

            std::cout << "new_value: " << new_value << " - old_value: "
                      << current_value << std::endl;
            //if(new_value < current_value || current_value < 0){
                labeling = std::move(new_labeling);
                current_value = new_value;
                debug_output(labeling, current_value);
                changed = true;
            //}

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
