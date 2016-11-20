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

#include "problem_lines.hxx"

#include <random>

#include <opencv2/imgproc/imgproc.hpp>

double
problem_lines::getNoiseLevel()
{
    return 0.03;
}

double
problem_lines::getNeighbourhoodWeight()
{
    return 0.1;
}

double
problem_lines::getHighlevelPriorsWeight()
{
    return 5.0;
}

size_t
problem_lines::getExpectedNumberOfStructures()
{
    return 10;
}

size_t
problem_lines::getNumberOfParametersPerHypothesis()
{
    // Taken from paper. Wrong IMO, as a line only has 2 parameters.
    return 3;
}

double
problem_lines::computeResidual(
    sample_type const & point, hypothesis_type const & line
) const {
    return line.dist(point);
}

double
problem_lines::getHypothesisCost(hypothesis_type const &){
    return 1;
}

double
problem_lines::getHypothesisInteractionCost( hypothesis_type const & l0,
                                             hypothesis_type const & l1 ){
    double dot = l0.dirx * l1.dirx + l0.diry * l1.diry;
    if(dot < 0) dot = -dot;
    if(dot < 0.995 && dot > 0.02)
        return 1;
    return 0;
}

void
problem_lines::debug_output(
    std::vector<MultiModelFitter_impl::label_type> const & labels,
    double value
)
{
    std::cout << "Value: " << value << std::endl;
    drawer.draw_labeled(labels);
    drawer.sleep(0);//100);
}

std::vector<std::array<MultiModelFitter_impl::sampleid_type,2>>
problem_lines::computeNeighbourhood(
    std::vector<sample_type> const & samples
) const
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

    std::cout << "x: " << x_min << " to " << x_max << std::endl;
    std::cout << "y: " << y_min << " to " << y_max << std::endl;

    // Feed data to Subdiv2D
    std::cout << "Still alive." << std::endl;
    cv::Rect rect(floor(x_min - 0.1f), floor(y_min - 0.1f), ceil(x_max + 0.1f) - floor(x_min - 0.1f), ceil(y_max + 0.1f) - floor(y_min - 0.1f));
    std::cout << "Rect: (" << rect.tl().x << "," << rect.tl().y << ") - ("
                           << rect.br().x << "," << rect.br().y << ") " << std::endl;
    cv::Subdiv2D subdiv(rect);
    std::vector<int> vertexIDs(samples.size());
    std::map<int, size_t> reverseVertexIDs;
    for(size_t i = 0; i < samples.size(); i++){
        auto const & sample = samples[i];
        std::cout << "s: " << sample.x << "-" << sample.y << std::endl;
        vertexIDs[i] = subdiv.insert(cv::Point2f(sample.x, sample.y));
        reverseVertexIDs[vertexIDs[i]] = i;
    }

    // Retrieve data
    typedef std::array<MultiModelFitter_impl::sampleid_type,2> conn_type;
    auto connections = std::vector<conn_type>();
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

std::vector<line_2d>
problem_lines::generateHypotheses(std::vector<point_2d> const & points,
                                  size_t num_hypotheses){

    // initialize random gen
    std::random_device rd;
    std::mt19937 gen(rd());
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
        std::vector<double> prob_weights(points.size());
        for(size_t i = 0; i < points.size(); i++){
            if(i == p1)
                prob_weights[i] = 0.0;
            else
                prob_weights[i] = points[p1].dist(points[i]);
        }

        // initialize random gen for second point
        std::piecewise_constant_distribution<> dist(std::begin(indices),
                                                    std::end(indices),
                                                    std::begin(prob_weights));

        // pick point2 by random
        size_t p2 = dist(gen);

        // generate line
        line_2d line(points[p1],points[p2]);

        // count number of inliers for this hypothesis
        size_t num_inliers = 0;
        for(size_t i = 0; i < points.size(); i++){
            double val = computeResidual(points[i], line);
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

