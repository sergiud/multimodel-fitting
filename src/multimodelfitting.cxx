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
#include <iostream>
#include <opengm/graphicalmodel/space/simplediscretespace.hxx>
#include <opengm/graphicalmodel/graphicalmodel.hxx>
#include <opengm/operations/adder.hxx>
#include <opengm/operations/minimizer.hxx>
#include <opengm/functions/potts.hxx>
#include <opengm/functions/explicit_function.hxx>
#include <opengm/inference/graphcut.hxx>
#include <opengm/inference/alphaexpansion.hxx>
#include <opengm/inference/auxiliary/minstcutkolmogorov.hxx>

std::vector<MultiModelFitter_impl::label_type> MultiModelFitter_impl::fit_impl() const
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
    
    std::cout << "Samples: " << sample_count << std::endl;
    std::cout << "Hypotheses: " << hypothesis_count << std::endl;


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
    
    // compute neighbourhood
    auto neighbourhood = getNeighbourhood(); 

    // Create fitting errors
    {
        // fetch noise level
        double noiseLevel = getNoiseLevel();
        double noiseLevelSquare = noiseLevel*noiseLevel;

        for(sampleid_type sample_id = 0; sample_id < sample_count; sample_id++){
            // Create one explicit 1d-funcion for every sample
            label_type shape[] = {hypothesis_count + 1};
            opengm::ExplicitFunction<double> f(shape, shape+1, 1.0f); 

            // outlier
            f(0) = 1.0;

            for(label_type j = 0; j < hypothesis_count; j++){
                // compute the residual between sample and hypothesis
                double residual = getResidual(sample_id, j);
                double penalty = (residual*residual)/noiseLevelSquare;
                f(j+1) = penalty;
            }

            // Register function
            GraphicalModelType::FunctionIdentifier fid = gm.addFunction(f);
            
            // Connect function
            sampleid_type fc[] = {sample_id};
            gm.addFactor(fid, fc, fc+1);  
        }
         
    }

    // Create smoothing errors
    {
        opengm::PottsFunction<double> f(hypothesis_count + 1, hypothesis_count + 1,
                                        0.0f, getNeighbourhoodWeight());  
        GraphicalModelType::FunctionIdentifier fid = gm.addFunction(f);

        for(auto const & neighbour : *neighbourhood){
            auto datap = neighbour.data();
            gm.addFactor(fid, datap, datap+2);
        }
    }
         
    
    
    typedef opengm::external::MinSTCutKolmogorov<label_type, double> MinStCutType;
    typedef opengm::GraphCut<GraphicalModelType, opengm::Minimizer, MinStCutType> MinGraphCut;
    typedef opengm::AlphaExpansion<GraphicalModelType, MinGraphCut> MinAlphaExpansion;
    
    MinAlphaExpansion ae(gm);
    std::cout << "Infering... " << std::endl;
    ae.infer();
    std::cout << "value: " << ae.value() << std::endl;

    std::cout << "test" << std::endl;
    std::vector<label_type> result;
    ae.arg(result);
    std::transform(result.begin(), result.end(), result.begin(),
                   [](label_type x){return x-1;});

    debug_output(result);
    return result;
}
//https://github.com/opengm/opengm
//cmakepackageconfighelpers
//siehe glog
