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

#include <mfigp/config.hxx>
#include "shapes_3d.hxx"

class problem_kinect : public mfigp::config<point_3d, plane_3d>{

public:
    void debug_output( std::vector<label_type> const &, computation_type );

    std::vector<std::array<sampleid_type,2>>
        computeNeighbourhood( std::vector<point_3d> const & );

    double computeResidual( point_3d const &, plane_3d const & );

    double getNoiseLevel();

    double getNeighbourhoodWeight();

    double getHighlevelPriorsWeight();

    size_t getExpectedNumberOfStructures();

    size_t getNumberOfParametersPerHypothesis();

    double getHypothesisCost( plane_3d const & );

    double getHypothesisInteractionCost( plane_3d const &,
                                         plane_3d const & );

    std::vector<plane_3d> generateHypotheses(std::vector<point_3d> const &,
                                             size_t, size_t, size_t);

};

