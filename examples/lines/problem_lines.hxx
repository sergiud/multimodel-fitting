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

#include "shapes_2d.hxx"

class problem_lines : public mfigp::config<point_2d, line_2d> {
public:

    void debug_output(std::vector<internal_label_type> const &, double);

    // Compute neighbourhood connections between samples.
    // In this case: delaunay triangulation
    std::vector<std::array<sampleid_type,2>>
        computeNeighbourhood(std::vector<sample_type> const &);

    // Compute the residual (Distance from point to hypothesis)
    // In this case : cross product between line direction and point direction
    // NOTE: A positive distance and a negative distance is both valid.
    double computeResidual(sample_type const &, hypothesis_type const &);

    // Returns the noise level.
    // If the residual of a point is larger than the noise level, it is
    // considered an outlier.
    double getNoiseLevel();

    // Returns the weight of the neighbourhood grouping.
    // Seems to work best in the range of 0.1 to 0.5.
    double getNeighbourhoodWeight();

    // Returns the weight of the highlevel priors.
    // Seems to work best in the range of 2.0 to 10.0
    double getHighlevelPriorsWeight();

    // Return the number of expected leftover hypothesis after fitting
    size_t getExpectedNumberOfStructures();

    // Returns the number of parameters every hypothesis needs to be fully
    // identified. Needed for weight computation.
    size_t getNumberOfParametersPerHypothesis();

    // Returns the cost of the hypothesis
    double getHypothesisCost(hypothesis_type const &);

    // Returns the cost of the interaction between two hypotheses
    double getHypothesisInteractionCost( hypothesis_type const &,
                                         hypothesis_type const & );

    // TODO decide on whether or not this shall be included in template
    std::vector<line_2d> generateHypotheses(std::vector<point_2d> const &, size_t);
};

