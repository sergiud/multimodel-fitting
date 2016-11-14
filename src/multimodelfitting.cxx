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
#include <opengm/graphicalmodel/space/simplediscretespace.hxx>
#include <opengm/graphicalmodel/graphicalmodel.hxx>
#include <opengm/operations/adder.hxx>
#include <opengm/operations/minimizer.hxx>
#include <opengm/functions/potts.hxx>
#include <opengm/functions/explicit_function.hxx>
#include <opengm/inference/graphcut.hxx>
#include <opengm/inference/alphaexpansion.hxx>
#include <opengm/inference/auxiliary/minstcutkolmogorov.hxx>

double MultiModelFitter_impl::compute_value(
    std::vector<MultiModelFitter_impl::label_type> labeling,
    double smoothing_penalty,
    std::shared_ptr<std::vector<std::array<MultiModelFitter_impl::sampleid_type, 2>>>
        const & neighbourhood,
    std::vector<std::vector<double>> const & fitting_penalties,
    std::vector<double> const & hypothesis_penalties,
    std::vector<std::vector<double>> const & hypothesis_interaction_penalties,
    double highlevel_constraint_weight
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
        result += hypothesis_penalties[label] * highlevel_constraint_weight;
    }

    // Hypothesis Interaction Penalties
    for(auto const & label1 : active_labels){
        for(auto const & label2 : active_labels){
            if(label1 >= label2) continue;
            result += hypothesis_interaction_penalties[label1][label2] * highlevel_constraint_weight;
        }
    }

    return result;
}

std::vector<unsigned char>
MultiModelFitter_impl::graph_cut(
    MultiModelFitter_impl::label_type alpha_label,
    std::vector<MultiModelFitter_impl::label_type> const & current_labeling,
    std::shared_ptr<std::vector<std::array<MultiModelFitter_impl::sampleid_type, 2>>>
        const & neighbourhood,
    std::vector<std::vector<double>> const & fitting_penalties,
    std::vector<double> const & hypothesis_penalties,
    std::vector<std::vector<double>> const & hypothesis_interaction_penalties
) const {

    sampleid_type sample_count = static_cast<sampleid_type>(current_labeling.size());

    // Create list of current labels
    std::set<label_type> previous_labels( current_labeling.begin(),
                                          current_labeling.end() );

    // construct space
    sampleid_type space_size = sample_count + previous_labels.size();
    if(previous_labels.count(alpha_label) == 0){
        space_size = space_size + 1;
    }
    typedef opengm::SimpleDiscreteSpace<sampleid_type, unsigned char> SpaceType;
    SpaceType space(space_size, 2);

    // construct gm
    typedef opengm::GraphicalModel<
        double,
        opengm::Adder,
        opengm::ExplicitFunction<double>,
        SpaceType
    > GraphicalModelType;
    GraphicalModelType gm(space);

    // Fitting penalties
    for(sampleid_type sample_id = 0; sample_id < sample_count; sample_id++){
        // Create one explicit 1d-funcion for every sample
        label_type shape[] = {2};
        opengm::ExplicitFunction<double> f(shape, shape+1);
        f(0) = fitting_penalties[sample_id][current_labeling[sample_id]];
        f(1) = fitting_penalties[sample_id][alpha_label];

        // Register function
        GraphicalModelType::FunctionIdentifier fid = gm.addFunction(f);

        // Connect function
        sampleid_type fc[] = {sample_id};
        gm.addFactor(fid, fc, fc+1);
    }

    // Smoothing penalties
    double smoothing_penalty = getNeighbourhoodWeight();
    for(auto const & neighbour : *neighbourhood){

        label_type shape[] = {2,2};
        opengm::ExplicitFunction<double> f(shape, shape+2);

        // If no change, take diff of current labeling
        if(current_labeling[neighbour[0]] == current_labeling[neighbour[1]])
            f(0,0) = 0;
        else
            f(0,0) = smoothing_penalty;

        // If both change, no penalty as both are alpha.
        f(1,1) = 0;

        // If label 1 is already alpha, no penalty for label 2 change
        if(current_labeling[neighbour[0]] == alpha_label)
            f(0,1) = 0;
        else
            f(0,1) = smoothing_penalty;

        // If label 2 is already alpha, no penalty for label 1 change
        if(current_labeling[neighbour[1]] == alpha_label)
            f(1,0) = 0;
        else
            f(1,0) = smoothing_penalty;

        GraphicalModelType::FunctionIdentifier fid = gm.addFunction(f);

        auto datap = neighbour.data();
        gm.addFactor(fid, datap, datap+2);
    }

    // Alpha Penalties
    double highlevel_constraint_weight = getHighlevelConstraintWeight();
    if(previous_labels.count(alpha_label) == 0)
    {
        double c_alpha = hypothesis_penalties[alpha_label];
        for(label_type const & previous_label : previous_labels){
            c_alpha += hypothesis_interaction_penalties[alpha_label][previous_label];
        }
        c_alpha *= highlevel_constraint_weight;

        // c_a(1-z_a)
        {
            label_type shape[] = {2};
            opengm::ExplicitFunction<double> f(shape, shape+1, 0.0);
            f(0) = c_alpha;
            f(1) = 0;
            GraphicalModelType::FunctionIdentifier fid = gm.addFunction(f);

            sampleid_type datap[] = {space_size - 1};
            gm.addFactor(fid, datap, datap+1);
        }

        // c_a(1-t_i)z_a
        for(sampleid_type i = 0; i < sample_count; i++){
            label_type shape[] = {2,2};
            opengm::ExplicitFunction<double> f(shape, shape+2, 0.0);
            f(0,0) = 0;
            f(0,1) = c_alpha;
            f(1,0) = 0;
            f(1,1) = 0;
            GraphicalModelType::FunctionIdentifier fid = gm.addFunction(f);

            sampleid_type datap[] = {i, space_size - 1};
            gm.addFactor(fid, datap, datap+2);
        }
        std::cout << "c_alpha: " << c_alpha << std::endl;
        std::cout << (space_size - 1) << "/" << space_size << std::endl;
    }

    // Beta Penalties
    {
        sampleid_type beta_position = sample_count;
        for(auto it = previous_labels.begin(); it != previous_labels.end(); it++)
        {
            label_type const & beta_label = *it;
            double c_beta = hypothesis_penalties[beta_label]
                     + hypothesis_interaction_penalties[beta_label][alpha_label];
            for( auto it2 = previous_labels.begin();
                 it2 != previous_labels.end(); it2++)
            {
                if(*it2 == beta_label)
                    continue;
                c_beta += 0.5*hypothesis_interaction_penalties[beta_label][*it2];
            }
            c_beta *= highlevel_constraint_weight;
            std::cout << "c_beta(" << *it << "): " << c_beta << std::endl;

            // c_b*z_b
            {
                label_type shape[] = {2};
                opengm::ExplicitFunction<double> f(shape, shape+1, 0.0);
                f(0) = 0;
                f(1) = c_beta;
                GraphicalModelType::FunctionIdentifier fid = gm.addFunction(f);

                sampleid_type datap[] = {beta_position};
                gm.addFactor(fid, datap, datap+1);
            }

            // c_b*t_i(1-z_b)
            for(sampleid_type i = 0; i < sample_count; i++){
                if(current_labeling[i] != beta_label) continue;
                label_type shape[] = {2,2};
                opengm::ExplicitFunction<double> f(shape, shape+2, 0.0);
                f(0,0) = 0;
                f(0,1) = 0;
                f(1,0) = c_beta;
                f(1,1) = 0;
                GraphicalModelType::FunctionIdentifier fid = gm.addFunction(f);

                sampleid_type datap[] = {i, beta_position};
                gm.addFactor(fid, datap, datap+2);
            }

            std::cout << beta_position << "/" << space_size << std::endl;
            beta_position++;
        }
    }

    // Solve
    typedef opengm::external::MinSTCutKolmogorov<label_type, double> MinStCutType;
    typedef opengm::GraphCut<GraphicalModelType, opengm::Minimizer, MinStCutType> MinGraphCut;

    MinGraphCut mincut(gm);
    mincut.infer();

    std::vector<unsigned char> result;
    mincut.arg(result);

    std::cout << result.size() << std::endl;

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
    // to hypothesis_count, and 0 for outliers. This is because labels in
    // opengm cannot be negative.


    sampleid_type sample_count = get_sample_count();
    label_type hypothesis_count = get_hypothesis_count();

    //////////////////////////////////////////
    // Fetch parameters

    // compute neighbourhood
    auto neighbourhood = getNeighbourhood();

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
    for(label_type i = 0; i < hypothesis_count + 1; i++){
        hypothesis_penalties[i] = getHypothesisCost(i);
    }

    // compute hypothesis interaction penalties
    std::vector<std::vector<double>> hypothesis_interaction_penalties(hypothesis_count + 1);
    for(label_type i = 0; i < hypothesis_count + 1; i++){
        std::vector<double> current_hypothesis_interaction_penalties(hypothesis_count + 1);
        for(label_type j = 0; j < hypothesis_count + 1; j++){
            current_hypothesis_interaction_penalties[j] =
                getHypothesisInteractionCost(i,j);
        }
        hypothesis_interaction_penalties[i] = std::move(current_hypothesis_interaction_penalties);
    }

    ////////////////////////////////////////
    // Create verification gm

    // construct space
    typedef opengm::SimpleDiscreteSpace<sampleid_type, label_type> SpaceType;
    SpaceType space(sample_count, hypothesis_count+1);

    // set function types
    typedef opengm::meta::TypeListGenerator<
        opengm::ExplicitFunction<double>,
        opengm::PottsFunction<double>
    >::type FunctionTypeList;

    // construct graphical model
    typedef opengm::GraphicalModel<
        double,
        opengm::Adder,
        FunctionTypeList,
        SpaceType
    > GraphicalModelType;
    GraphicalModelType gm(space);

    // Create fitting errors
    for(sampleid_type sample_id = 0; sample_id < sample_count; sample_id++){
        // Create one explicit 1d-funcion for every sample
        label_type shape[] = {hypothesis_count + 1};
        opengm::ExplicitFunction<double> f(shape, shape+1);

        // Set weights
        for(label_type j = 0; j < hypothesis_count+1; j++){
            f(j) = fitting_penalties[sample_id][j];
        }

        // Register function
        GraphicalModelType::FunctionIdentifier fid = gm.addFunction(f);

        // Connect function
        sampleid_type fc[] = {sample_id};
        gm.addFactor(fid, fc, fc+1);
    }

    // Create smoothing errors
    double smoothing_penalty = getNeighbourhoodWeight();
    {
        opengm::PottsFunction<double> f( hypothesis_count + 1,
                                         hypothesis_count + 1, 
                                         0.0,
                                         smoothing_penalty );

        // Register function
        GraphicalModelType::FunctionIdentifier fid = gm.addFunction(f);

        for(sampleid_type sample1 = 0; sample1 < sample_count; sample1++){
            for(sampleid_type sample2 = 0; sample2 < sample_count; sample2++){
                // Connect function
                if(sample1 >= sample2) continue;
                sampleid_type fc[] = {sample1, sample2};
                gm.addFactor(fid, fc, fc+2);
            }
        }
    }


    ////////////////////////////////////////
    // Solve

    // Alpha Expansion algorithm
    std::vector<label_type> labeling(sample_count);
    for(sampleid_type i = 0; i < labeling.size(); i++){
        labeling[i] = 0;//i%(hypothesis_count+1);
    }

    bool changed = false;
    double current_value = -1;//gm.evaluate(labeling);
    debug_output(labeling, current_value);
    do {
        changed = false;

        for(label_type alpha_label = 0; alpha_label < hypothesis_count + 1; alpha_label++)
        {
            std::cout << "-------------" << std::endl;
            std::cout << "alpha: " << (hypothesis_count - alpha_label) << std::endl;
            // Run graph cut
            auto alpha_expansion_suggestion = graph_cut( hypothesis_count - alpha_label,
                                                         labeling,
                                                         neighbourhood,
                                                         fitting_penalties,
                                                         hypothesis_penalties,
                                                         hypothesis_interaction_penalties );

            for(size_t i = sample_count; i < alpha_expansion_suggestion.size(); i++){
                std::cout << "z_" << i << " = " << static_cast<int>(alpha_expansion_suggestion[i]) << std::endl;
            }

            // Create potential new labeling
            std::vector<label_type> new_labeling;
            new_labeling.reserve(sample_count);
            for(sampleid_type i = 0; i < sample_count; i++){
                if(alpha_expansion_suggestion[i] == 0){
                    new_labeling.push_back(labeling[i]);
                } else {
                    new_labeling.push_back(hypothesis_count - alpha_label);
                }
            }

            // Swap depending on wether or not the new labeling is actually better
            double new_value = gm.evaluate(new_labeling);
            std::cout << "new_value_nohighlevel: " << new_value << std::endl;
            double highlevel_constraint_weight = getHighlevelConstraintWeight();
            std::set<label_type> active_labels( new_labeling.begin(),
                                                new_labeling.end() );
            for( auto const & active_label1 : active_labels ){
                new_value += hypothesis_penalties[active_label1]
                             * highlevel_constraint_weight;
                for( auto const & active_label2 : active_labels ){
                    if(active_label1 >= active_label2) continue;
                    std::cout << " - " << active_label1 << " - "
                              << active_label2 << " : ";
                    new_value += hypothesis_interaction_penalties[active_label1]
                                                                 [active_label2]
                                 * highlevel_constraint_weight;
                    std::cout << (hypothesis_interaction_penalties[active_label1]
                                                                 [active_label2]
                                 * highlevel_constraint_weight) << std::endl;
                }
            }
            std::cout << "new_value: " << new_value << " - old_value: "
                      << current_value << std::endl;
            std::cout << "our computed value: "
                      << compute_value( new_labeling,
                                        smoothing_penalty,
                                        neighbourhood,
                                        fitting_penalties,
                                        hypothesis_penalties,
                                        hypothesis_interaction_penalties,
                                        highlevel_constraint_weight )
                      << std::endl;
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
//https://github.com/opengm/opengm
//cmakepackageconfighelpers
//siehe glog
