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

#include <vector>
#include <array>
#include <cstdint>

namespace mfigp {

template<class sample_type_,
         class hypothesis_type_,
         typename computation_type_ = double,
         typename label_type_ = int32_t,
         typename sampleid_type_ = size_t>
class config {

    public:
        typedef sample_type_        sample_type;
        typedef hypothesis_type_    hypothesis_type;
        typedef computation_type_   computation_type;
        typedef label_type_         label_type;
        typedef sampleid_type_      sampleid_type;

    public:

        // This callback will get called whenever the labeling changed
        // during the fitting process.
        // !! IMPORTANT !!
        // The labels that get passed through are shifted by one, namely:
        // 0 - outlier
        // 1 - label 0
        // 2 - label 1
        virtual void
            debug_output( std::vector<label_type> const &, computation_type ) = 0;

        // Compute neighbourhood connections between samples.
        // For example: delaunay triangulation
        virtual std::vector<std::array<sampleid_type,2>>
            computeNeighbourhood( std::vector<sample_type> const & ) = 0;

        // Compute the residual (Distance from point to hypothesis)
        // In this case : cross product between line direction and point direction
        // NOTE: A positive distance and a negative distance is both valid.
        virtual computation_type
            computeResidual( sample_type const &, hypothesis_type const & ) = 0;

        // Returns the noise level.
        // If the residual of a point is larger than the noise level, it is
        // considered an outlier.
        virtual computation_type
            getNoiseLevel() = 0;

        // Returns the weight of the neighbourhood grouping.
        // Seems to work best in the range of 0.1 to 0.5.
        virtual computation_type
            getNeighbourhoodWeight() = 0;

        // Returns the weight of the highlevel priors.
        // Seems to work best in the range of 2.0 to 10.0
        virtual computation_type
            getHighlevelPriorsWeight() = 0;

        // Return the number of expected leftover hypothesis after fitting
        virtual size_t
            getExpectedNumberOfStructures() = 0;

        // Returns the number of parameters every hypothesis needs to be fully
        // identified. Needed for weight computation.
        virtual size_t
            getNumberOfParametersPerHypothesis() = 0;

        // Returns the cost of the hypothesis
        virtual computation_type
            getHypothesisCost( hypothesis_type const & ) = 0;

        // Returns the cost of the interaction between two hypotheses
        virtual computation_type
            getHypothesisInteractionCost( hypothesis_type const &,
                                          hypothesis_type const & ) = 0;

};

}
