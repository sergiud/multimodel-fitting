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

    // construct space
    typedef opengm::SimpleDiscreteSpace<sampleid_type, unsigned char> SpaceType;
    SpaceType space(sample_count, 2);
    
    // construct gm
    typedef opengm::GraphicalModel<
        double,
        opengm::Adder,
        opengm::ExplicitFunction<double>,
        SpaceType
    > GraphicalModelType;
    GraphicalModelType gm(space);

    // Fitting errors
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

    // Smoothing errors
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

    // TODO other errors

    // Solve
    typedef opengm::external::MinSTCutKolmogorov<label_type, double> MinStCutType;
    typedef opengm::GraphCut<GraphicalModelType, opengm::Minimizer, MinStCutType> MinGraphCut;
 
    MinGraphCut mincut(gm);
    mincut.infer();

    std::vector<unsigned char> result;
    mincut.arg(result);

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
         

    ////////////////////////////////////////
    // Solve

    // Alpha Expansion algorithm
    std::vector<label_type> labeling(sample_count);
    std::fill(labeling.begin(), labeling.end(), 0);
    
    bool changed = false;
    double current_value = gm.evaluate(labeling);
    debug_output(labeling, current_value);
    do {
        changed = false;
        
        for(label_type alpha_label = 0; alpha_label < hypothesis_count + 1; alpha_label++)
        {
            // Run graph cut
            auto alpha_expansion_suggestion = graph_cut( alpha_label,
                                                         labeling,
                                                         neighbourhood,
                                                         fitting_penalties,
                                                         hypothesis_penalties,
                                                         hypothesis_interaction_penalties );
            
            // Create potential new labeling
            std::vector<label_type> new_labeling;
            new_labeling.reserve(sample_count); 
            for(sampleid_type i = 0; i < sample_count; i++){
                if(alpha_expansion_suggestion[i] == 0){
                    new_labeling.push_back(labeling[i]);
                } else {
                    new_labeling.push_back(alpha_label);
                }
            }

            // Swap depending on wether or not the new labeling is actually better
            double new_value = gm.evaluate(new_labeling);
            if(new_value < current_value){
                labeling = std::move(new_labeling);
                current_value = new_value;
                debug_output(labeling, current_value);
                changed = true;
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
//https://github.com/opengm/opengm
//cmakepackageconfighelpers
//siehe glog
