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

/**
 * \brief Configuration class template
 *
 * This class is meant to be derived by the config classes for the
 * MultiModelFitter class.
 *
 * \tparam sample_type_         The type of the samples.<BR>
 *                              This can be any arbitrary type.
 * \tparam hypothesis_type_     The type of the hypotheses.<BR>
 *                              This can be any arbitrary type.
 * \tparam computation_type_    The precision type for the actual computaton.<BR>
 *                              This is usually double or float, but can also
 *                              be an integer type.
 * \tparam label_type_          The type of the labels that maps samples to
 *                              hypotheses.<BR>
 *                              This needs to be signed, as the outlier label
 *                              is represented by -1.
 * \tparam sampleid_type_       The id-type to identify a sample.<BR>
 *                              In most cases, this should be size_t.
 */
template<class sample_type_,
         class hypothesis_type_,
         typename computation_type_ = double,
         typename label_type_ = int32_t,
         typename sampleid_type_ = size_t>
class Config {

    public:
        /** \brief Type of the samples. */
        typedef sample_type_        sample_type;
        /** \brief Type of the hypotheses. */
        typedef hypothesis_type_    hypothesis_type;
        /** \brief Type used for computation. */
        typedef computation_type_   computation_type;
        /**
         * \brief Type used to represent labels.
         *
         * A label is the hypothesis id a certain sample belongs to.
         *
         * Always signed.<BR>Outliers are -1.
         */
        typedef label_type_         label_type;
        /** \brief Type used to identify samples. */
        typedef sampleid_type_      sampleid_type;
        /** \brief Type internally used to represent labels.
         *
         * Always unsigned.<BR>Outliers are 0.<BR>
         * Label _i_ has internal label _i+1_.
         */
        typedef typename std::make_unsigned<label_type>::type
                                    internal_label_type;

    public:

        /**
         * \brief Debug callback.
         *
         * \param labels The currently assigned labels.<BR>
         *               __IMPORTANT__: the type is internal_label_type,
         *               which is unsigned and therefore shifted by one.
         * \param cost The cost value of the current labeling.
         *
         * This callback will get called whenever the labeling changed
         * during the fitting process.
         *
         * __IMPORTANT__:
         * The labels that get passed through are shifted by one, namely:
         *    - 0 - outlier
         *    - 1 - label 0
         *    - 2 - label 1
         */
        virtual void
            debug_output( std::vector<internal_label_type> const & labels,
                          computation_type cost ) = 0;

        /**
         * \brief Compute neighbourhood connections between samples.
         *
         * \return A vector of neighbour pairs.
         *
         * _For example_: delaunay triangulation
         */
        virtual std::vector<std::array<sampleid_type,2>>
            computeNeighbourhood( std::vector<sample_type> const & ) = 0;

        /**
         * \brief Compute the residual (Distance from sample to hypothesis)
         *
         * _Lines example_: cross product between line direction and point direction
         *
         * __NOTE__: A positive distance and a negative distance is both valid.
         */
        virtual computation_type
            computeResidual( sample_type const &, hypothesis_type const & ) = 0;

        /**
         * \brief Returns the noise level.
         *
         * If the residual of a point is larger than the noise level, it is
         * considered an outlier.
         */
        virtual computation_type
            getNoiseLevel() = 0;

        /**
         * \brief Returns the weight of the neighbourhood grouping.
         *
         * Seems to work best in the range of 0.1 to 0.5.
         */
        virtual computation_type
            getNeighbourhoodWeight() = 0;

        /**
         * \brief Returns the weight of the highlevel priors.
         *
         * Seems to work best in the range of 2.0 to 10.0
         */
        virtual computation_type
            getHighlevelPriorsWeight() = 0;

        /** \brief Return the number of expected leftover hypothesis after fitting. */
        virtual size_t
            getExpectedNumberOfStructures() = 0;

        /**
         * \brief Returns the number of parameters every hypothesis needs
         *        so it is fully identified.
         *
         * Needed for weight computation.
         *
         * _Examples_:
         *     - 2D line:   2
         *     - 3D plane:  3
         *     - 3D line:   4
         *
         */
        virtual size_t
            getNumberOfParametersPerHypothesis() = 0;

        /**
         * \brief Returns the cost of the hypothesis.
         *
         * \return The cost of a hypothesis. Should be between 0 and 1.
         */
        virtual computation_type
            getHypothesisCost( hypothesis_type const & ) = 0;

        /**
         * \brief Returns the cost of the interaction between two hypotheses.
         *
         * \return The interaction cost of the two hypothesis.<BR>
         *         Should be between 0 and 1, where 0 means completely compatible
         *         and 1 means totally incompatible.
         */
        virtual computation_type
            getHypothesisInteractionCost( hypothesis_type const &,
                                          hypothesis_type const & ) = 0;

};

}
