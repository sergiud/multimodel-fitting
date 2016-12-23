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

#include "../../examples/lines/shapes_2d.hxx"

template<typename computation_type_>
class problem_lines : public mfigp::Config<point_2d, line_2d, computation_type_> {
public:

    typedef mfigp::Config<point_2d, line_2d, computation_type_> parent_class;
    typedef typename parent_class::sample_type           sample_type;
    typedef typename parent_class::hypothesis_type       hypothesis_type;
    typedef typename parent_class::computation_type      computation_type;
    typedef typename parent_class::label_type            label_type;
    typedef typename parent_class::sampleid_type         sampleid_type;
    typedef typename parent_class::internal_label_type   internal_label_type;

    void debug_output(std::vector<internal_label_type> const &, computation_type);

    // Compute neighbourhood connections between samples.
    // In this case: delaunay triangulation
    std::vector<std::array<sampleid_type,2>>
        computeNeighbourhood(std::vector<sample_type> const &);

    // Compute the residual (Distance from point to hypothesis)
    // In this case : cross product between line direction and point direction
    // NOTE: A positive distance and a negative distance is both valid.
    computation_type computeResidual(sample_type const &, hypothesis_type const &);

    // Returns the noise level.
    // If the residual of a point is larger than the noise level, it is
    // considered an outlier.
    computation_type getNoiseLevel();

    // Returns the weight of the neighbourhood grouping.
    // Seems to work best in the range of 0.1 to 0.5.
    computation_type getNeighbourhoodWeight();

    // Returns the weight of the highlevel priors.
    // Seems to work best in the range of 2.0 to 10.0
    computation_type getHighlevelPriorsWeight();

    // Return the number of expected leftover hypothesis after fitting
    size_t getExpectedNumberOfStructures();

    // Returns the number of parameters every hypothesis needs to be fully
    // identified. Needed for weight computation.
    size_t getNumberOfParametersPerHypothesis();

    // Returns the cost of the hypothesis
    computation_type getHypothesisCost(hypothesis_type const &);

    // Returns the cost of the interaction between two hypotheses
    computation_type getHypothesisInteractionCost( hypothesis_type const &,
                                         hypothesis_type const & );

    // TODO decide on whether or not this shall be included in template
    std::vector<line_2d> generateHypotheses(std::vector<point_2d> const &, size_t);
};


#include <map>
#include <random>
#include <iostream>

#include <opencv2/imgproc/imgproc.hpp>

template<typename computation_type_>
typename problem_lines<computation_type_>::computation_type
problem_lines<computation_type_>::getNoiseLevel()
{
    return computation_type(0.03);
}

template<typename computation_type_>
typename problem_lines<computation_type_>::computation_type
problem_lines<computation_type_>::getNeighbourhoodWeight()
{
    return computation_type(0.1);
}

template<typename computation_type_>
typename problem_lines<computation_type_>::computation_type
problem_lines<computation_type_>::getHighlevelPriorsWeight()
{
    return computation_type(5.0);
}

template<typename computation_type_>
size_t
problem_lines<computation_type_>::getExpectedNumberOfStructures()
{
    return 10;
}

template<typename computation_type_>
size_t
problem_lines<computation_type_>::getNumberOfParametersPerHypothesis()
{
    // Taken from paper. Wrong IMO, as a line only has 2 parameters.
    return 3;
}

template<typename computation_type_>
typename problem_lines<computation_type_>::computation_type
problem_lines<computation_type_>::computeResidual(
    sample_type const & point, hypothesis_type const & line
) {
    return computation_type(line.dist(point));
}

template<typename computation_type_>
typename problem_lines<computation_type_>::computation_type
problem_lines<computation_type_>::getHypothesisCost(hypothesis_type const &){
    return computation_type(1);
}

template<typename computation_type_>
typename problem_lines<computation_type_>::computation_type
problem_lines<computation_type_>::getHypothesisInteractionCost(
                                            hypothesis_type const & l0,
                                            hypothesis_type const & l1 ){
    computation_type dot = computation_type(l0.dirx * l1.dirx + l0.diry * l1.diry);
    computation_type cross = computation_type(l0.dirx * l1.diry - l0.diry * l1.dirx);

    computation_type dot2 = dot*dot;
    computation_type cross2 = cross*cross;

    computation_type val = dot2;
    if (dot2 > cross2) {
        val = cross2;
    }

    computation_type scalar = 2500;
    val *= scalar;

    if (val > 1)
        val = 1;

    assert(val >= 0 && val <= 1);

    return val;
}

template<typename computation_type_>
void
problem_lines<computation_type_>::debug_output(
    std::vector<internal_label_type> const &,
    computation_type value
)
{
    std::cout << "Value: " << value << std::endl;
}

template<typename computation_type_>
std::vector<std::array<typename problem_lines<computation_type_>::sampleid_type,2>>
problem_lines<computation_type_>::computeNeighbourhood(
    std::vector<sample_type> const & samples
)
{
    float x_max = samples[0].x;
    float y_max = samples[0].y;
    float x_min = x_max;
    float y_min = y_max;
    for(auto const & sample : samples){
        if(sample.x > x_max) x_max = sample.x;
        if(sample.x < x_min) x_min = sample.x;
        if(sample.y > y_max) y_max = sample.y;
        if(sample.y < y_min) y_min = sample.y;
    }

    // Feed data to Subdiv2D
    cv::Rect rect(int(std::floor(x_min - 0.1f)), int(std::floor(y_min - 0.1f)),
                  int(std::ceil(x_max + 0.1f) - std::floor(x_min - 0.1f)),
                  int(std::ceil(y_max + 0.1f) - std::floor(y_min - 0.1f)));
    cv::Subdiv2D subdiv(rect);
    std::vector<int> vertexIDs(samples.size());
    std::map<int, size_t> reverseVertexIDs;
    for(size_t i = 0; i < samples.size(); i++){
        auto const & sample = samples[i];
        vertexIDs[i] = subdiv.insert(cv::Point2f(sample.x, sample.y));
        reverseVertexIDs[vertexIDs[i]] = i;
    }

    // Retrieve data
    typedef std::array<sampleid_type,2> conn_type;
    auto connections = std::vector<conn_type>();
    for(size_t i = 0; i < samples.size(); i++){
        auto const & vertexID = vertexIDs[i];
        int edgeID;
        subdiv.getVertex(vertexID, &edgeID);
        int nextEdgeID = edgeID;
        do{
            int targetID = subdiv.edgeDst(nextEdgeID);
            if(targetID > vertexID){
                conn_type connection = {{
                    reverseVertexIDs[vertexID],
                    reverseVertexIDs[targetID]
                }};
                // Only allow neighbours with a distance of at most 0.1
                if(samples[connection[0]].dist(samples[connection[1]]) <= 0.1f){
                    connections.push_back(connection);
                }
            }
            nextEdgeID = subdiv.nextEdge(nextEdgeID);
        } while(nextEdgeID != edgeID);
    }

    return connections;
}

template<typename computation_type_>
std::vector<line_2d>
problem_lines<computation_type_>::generateHypotheses(std::vector<point_2d> const & points,
                                  size_t num_hypotheses){

    // initialize random gen
    std::mt19937 gen(2);
    auto rnd_int = std::uniform_int_distribution<size_t>(0,points.size()-1);

    // Set up indices for random number generator
    std::vector<size_t> indices(points.size());
    for(size_t i = 0; i < points.size(); i++){
        indices[i] = i;
    }

    // initialize target data structure
    std::vector<line_2d> lines;
    lines.reserve(num_hypotheses);

    for(size_t i = 0; i < num_hypotheses; i++){

        // pick point1 by random
        size_t p1 = rnd_int(gen);

        // generate propabilities for second point
        std::vector<computation_type> prob_weights(points.size());
        for(size_t j = 0; j < points.size(); j++){
            if(j == p1)
                prob_weights[j] = 0.0;
            else
                prob_weights[j] = computation_type(points[p1].dist(points[j]));
        }

        // initialize random gen for second point
        std::piecewise_constant_distribution<> dist(std::begin(indices),
                                                    std::end(indices),
                                                    std::begin(prob_weights));

        // pick point2 by random
        size_t p2 = size_t(dist(gen));

        // generate line
        line_2d line(points[p1],points[p2]);

        // count number of inliers for this hypothesis
        size_t num_inliers = 0;
        for(size_t j = 0; j < points.size(); j++){
            computation_type val = computeResidual(points[j], line);
            if (val < 0) val = -val;
            if(val < getNoiseLevel()){
                num_inliers++;
            }
        }

        // discard if too little inliers
        if(num_inliers <= 30)
            continue;

        lines.push_back(std::move(line));
    }

    return lines;
}

