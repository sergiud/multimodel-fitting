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

#include <multimodelfitting.hxx>
#include "problem_ortholines.hxx"

#include <iostream>
#include <map>

#include <opencv2/imgproc/imgproc.hpp>

problem_ortholines::problem_ortholines(std::shared_ptr<drawer_2d> drawer)
{
    this->drawer = drawer;
}

void problem_ortholines::debug_output(
    std::vector<MultiModelFitter_impl::label_type> const & labels,
    double value
)
{
    std::cout << "Value: " << value << std::endl;
    drawer->draw_labeled(labels);
    drawer->sleep(1);
}

std::shared_ptr<std::vector<std::array<MultiModelFitter_impl::sampleid_type,2>>>
problem_ortholines::computeNeighbourhood(
    std::vector<sample_type> const & samples
) const
{
    typedef std::array<MultiModelFitter_impl::sampleid_type,2> conn_type;
    // Compute bounding box
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
    cv::Rect rect(x_min - 0.1f, y_min - 0.1f, x_max + 0.1f, y_max + 0.1f);
    cv::Subdiv2D subdiv(rect);
    std::vector<int> vertexIDs(samples.size());
    std::map<int, size_t> reverseVertexIDs;
    for(size_t i = 0; i < samples.size(); i++){
        auto const & sample = samples[i];
        vertexIDs[i] = subdiv.insert(cv::Point2f(sample.x, sample.y));
        reverseVertexIDs[vertexIDs[i]] = i;
    }

    // Retrieve data
    auto connections = std::make_shared<std::vector<conn_type>>();
    for(size_t i = 0; i < samples.size(); i++){
        auto const & vertexID = vertexIDs[i];
        int edgeID;
        subdiv.getVertex(vertexID, &edgeID);
        int nextEdgeID = edgeID;
        do{
            int targetID = subdiv.edgeDst(nextEdgeID);
            if(targetID > vertexID){
                conn_type connection = {
                    reverseVertexIDs[vertexID],
                    reverseVertexIDs[targetID]
                };
                connections->push_back(connection);
            }
            nextEdgeID = subdiv.nextEdge(nextEdgeID);
        } while(nextEdgeID != edgeID);
    }

    return connections;
}


double
problem_ortholines::computeResidual(
    sample_type const & point, hypothesis_type const & line
) const {
    return line.dist(point);
}

double
problem_ortholines::getNoiseLevel()
{
    return 0.1;
}

double
problem_ortholines::getNeighbourhoodWeight()
{
    return 0.25;
}

// Returns the cost of the hypothesis
double
problem_ortholines::getHypothesisCost(hypothesis_type const &)
{
    return 1.0;
}

// Returns the cost of the interaction between two hypotheses
double
problem_ortholines::getHypothesisInteractionCost( hypothesis_type const & line1,
                                                  hypothesis_type const & line2 )
{
    // compute dotproduct of direction normal vectors
    double dotproduct = line1.dirx * line2.dirx + line1.diry * line2.diry;

    // abs
    if(dotproduct < 0)
        dotproduct = -dotproduct;

    double angle = 180.0 * dotproduct / 3.14159265358979323846;

    double cost = exp(angle) - 1;

    if (cost > 10000)
        return 10000;

    return cost;
}




