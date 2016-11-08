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

#include<multimodelfitting.hxx>
#include "shapes_2d.hxx"
#include "drawer_2d.hxx"

class problem_ortholines {
public:
    typedef point_2d sample_type;
    typedef line_2d hypothesis_type;
public:
    problem_ortholines(std::shared_ptr<drawer_2d> drawer);

    void debug_output(std::vector<MultiModelFitter_impl::label_type> const &);

    // Compute neighbourhood connections between samples.
    // In this case: delaunay triangulation
    std::shared_ptr<std::vector<std::array<MultiModelFitter_impl::sampleid_type,2>>>
        computeNeighbourhood(std::vector<sample_type> const &) const; 

    // Compute the residual (Distance from point to hypothesis)
    // In this case : cross product between line direction and point direction
    // NOTE: A positive distance and a negative distance is both valid.
    double computeResidual(sample_type const &, hypothesis_type const &) const;

    // Returns the noise level.
    // If the residual of a point is larger than the noise level, it is
    // considered an outlier.
    double getNoiseLevel(); 

private:
    std::shared_ptr<drawer_2d> drawer;
};
