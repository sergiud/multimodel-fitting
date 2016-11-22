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

#include "problem_kinect.hxx"
#include <iostream>

void
problem_kinect::debug_output( std::vector<label_type> const & labeling,
                              computation_type value ){
    std::cout << "Value: " << value << std::endl;
}

std::vector<std::array<problem_kinect::sampleid_type,2>>
problem_kinect::computeNeighbourhood( std::vector<point_3d> const & ){
    return std::vector<std::array<sampleid_type,2>>();
}

double
problem_kinect::computeResidual( point_3d const & p, plane_3d const & h ){
    return 0;//h.dist(p);
}

double
problem_kinect::getNoiseLevel(){
    // TODO
    return 0;
}

double
problem_kinect::getNeighbourhoodWeight(){
    // TODO
    return 0;
}

double
problem_kinect::getHighlevelPriorsWeight(){
    // TODO
    return 0;
}

size_t
problem_kinect::getExpectedNumberOfStructures(){
    return 10;
}

size_t
problem_kinect::getNumberOfParametersPerHypothesis(){
    // A 3d plane can be identified by 3 parameters
    return 3;
}

double
problem_kinect::getHypothesisCost( plane_3d const & ){
    // TODO
    return 0;
}

double
problem_kinect::getHypothesisInteractionCost( plane_3d const &,
                                              plane_3d const & ){
    // TODO
    return 0;
}

std::vector<plane_3d>
problem_kinect::generateHypotheses(std::vector<point_3d> const &, size_t)
{
    return std::vector<plane_3d>();
}

