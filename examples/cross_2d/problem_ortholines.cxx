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

#include "problem_ortholines.hxx"

#include <iostream>

#include <opencv2/imgproc/imgproc.hpp>

problem_ortholines::problem_ortholines(std::shared_ptr<drawer_2d> drawer)
{
    this->drawer = drawer;
}

void problem_ortholines::debug_output(
    std::vector<MultiModelFitter_impl::label_type> const & labels
)
{
    drawer->draw_labeled(labels);
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
    float dist_x = point.x - line.x0;
    float dist_y = point.y - line.y0;
    return dist_x * line.diry - dist_y * line.dirx;
}

double
problem_ortholines::getNoiseLevel(){
    return 0.05;
}
