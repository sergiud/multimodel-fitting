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

#include <map>
#include <random>
#include <iostream>
#include "drawer_2d.hxx"

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
) {
    return double(line.dist(point));
}

double
problem_lines::getHypothesisCost(hypothesis_type const &){
    return 1;
}

double
problem_lines::getHypothesisInteractionCost( hypothesis_type const & l0,
                                             hypothesis_type const & l1 ){
    double dot = double(l0.dirx * l1.dirx + l0.diry * l1.diry);
    double cross = double(l0.dirx * l1.diry - l0.diry * l1.dirx);

    double dot2 = dot*dot;
    double cross2 = cross*cross;

    double val = dot2;
    if (dot2 > cross2) {
        val = cross2;
    }

    double scalar = 2500;
    val *= scalar;

    if (val > 1)
        val = 1;

    assert(val >= 0 && val <= 1);

    return val;
}

void
problem_lines::debug_output(
    std::vector<internal_label_type> const & labels,
    double value
)
{
    std::cout << "Value: " << value << std::endl;

    std::vector<label_type> transformed_labels(labels.size());
    std::transform(labels.begin(), labels.end(), transformed_labels.begin(),
                   [](internal_label_type x){return label_type(x)-1;});

    drawer_2d& drawer = drawer_2d::get_instance();
    drawer.draw_labeled(transformed_labels);
    drawer.sleep(100);
}

std::vector<std::array<problem_lines::sampleid_type,2>>
problem_lines::computeNeighbourhood(
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
        for(size_t j = 0; j < points.size(); j++){
            if(j == p1)
                prob_weights[j] = 0.0;
            else
                prob_weights[j] = double(points[p1].dist(points[j]));
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
            double val = computeResidual(points[j], line);
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

