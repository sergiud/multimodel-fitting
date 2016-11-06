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
//#include <opengm/graphicalmodel/graphicalmodel.hxx>
//#include <opengm/operations/adder.hxx>
//#include <opengm/operations/minimizer.hxx>
//#include <opengm/inference/graphcut.hxx>
//#include <opengm/inference/alphaexpansion.hxx>
//#include <opengm/inference/auxiliary/minstcutkolmogorov.hxx>

std::vector<MultiModelFitter_impl::label_type> MultiModelFitter_impl::fit_impl() const
{
    size_t sample_count = get_sample_count();
    label_type hypothesis_count = get_hypothesis_count(); 

    /*typedef opengm::GraphicalModel<double, opengm::Adder> GraphicalModelType;
    typedef opengm::external::MinSTCutKolmogorov<size_t, double> MinStCutType;
    typedef opengm::GraphCut<GraphicalModelType, opengm::Minimizer, MinStCutType> MinGraphCut;
    typedef opengm::AlphaExpansion<GraphicalModelType, MinGraphCut> MinAlphaExpansion;
    GraphicalModelType gm;
    */
    
    // our space
    opengm::SimpleDiscreteSpace<> space(sample_count, hypothesis_count);

    // ... 13
    /*MinAlphaExpansion ae(gm);
    ae.infer();
    std::cout << "value: " << ae.value() << std::endl;
    */
    std::cout << "test" << std::endl;
    return std::vector<MultiModelFitter_impl::label_type>();
}
//https://github.com/opengm/opengm
//cmakepackageconfighelpers
//siehe glog
