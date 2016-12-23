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
#include "../../examples/kinect/shapes_3d.hxx"

template<typename computation_type_>
class problem_kinect : public mfigp::Config<point_3d, plane_3d, computation_type_>{

public:

    typedef mfigp::Config<point_3d, plane_3d, computation_type_> parent_class;
    typedef typename parent_class::sample_type           sample_type;
    typedef typename parent_class::hypothesis_type       hypothesis_type;
    typedef typename parent_class::computation_type      computation_type;
    typedef typename parent_class::label_type            label_type;
    typedef typename parent_class::sampleid_type         sampleid_type;
    typedef typename parent_class::internal_label_type   internal_label_type;


    void debug_output( std::vector<internal_label_type> const &, computation_type );

    std::vector<std::array<sampleid_type,2>>
        computeNeighbourhood( std::vector<point_3d> const & );

    computation_type computeResidual( point_3d const &, plane_3d const & );

    computation_type getNoiseLevel();

    computation_type getNeighbourhoodWeight();

    computation_type getHighlevelPriorsWeight();

    size_t getExpectedNumberOfStructures();

    size_t getNumberOfParametersPerHypothesis();

    computation_type getHypothesisCost( plane_3d const & );

    computation_type getHypothesisInteractionCost( plane_3d const &,
                                         plane_3d const & );

    std::vector<plane_3d> generateHypotheses(std::vector<point_3d> const &,
                                             size_t);

};

#include <cassert>
#include <iostream>
#include <random>

#include <opencv2/opencv.hpp>

template<typename computation_type_>
void
problem_kinect<computation_type_>::debug_output( std::vector<internal_label_type> const &,
                              computation_type value ){
    std::cout << "Value: " << value << std::endl;
}

extern unsigned char img_depth[];
extern size_t img_depth_len;

template<typename computation_type_>
std::vector<std::array<typename problem_kinect<computation_type_>::sampleid_type,2>>
problem_kinect<computation_type_>::computeNeighbourhood( std::vector<point_3d> const & samples ){

    int x_max = samples[0].u;
    int y_max = samples[0].v;
    int x_min = x_max;
    int y_min = y_max;
    for(auto const & sample : samples){
        if(sample.u > x_max) x_max = sample.u;
        if(sample.u < x_min) x_min = sample.u;
        if(sample.v > y_max) y_max = sample.v;
        if(sample.v < y_min) y_min = sample.v;
    }

    // Feed data to Subdiv2D
    const float epsilon = 0.1f;
    cv::Rect rect( int(std::floor(x_min - epsilon)),
                   int(std::floor(y_min - epsilon)),
                   int(std::ceil(x_max + epsilon) - std::floor(x_min - epsilon)),
                   int(std::ceil(y_max + epsilon) - std::floor(y_min - epsilon)));
    cv::Subdiv2D subdiv(rect);
    std::vector<int> vertexIDs(samples.size());
    std::map<int, size_t> reverseVertexIDs;
    for(size_t i = 0; i < samples.size(); i++){
        auto const & sample = samples[i];
        vertexIDs[i] = subdiv.insert(cv::Point2f(float(sample.u), float(sample.v)));
        reverseVertexIDs[vertexIDs[i]] = i;
    }

    // Threshold for pixels that are too far apart
    const float threshold_factor = 10;

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
                // Compute distance
                auto const & p0 = samples[connection[0]];
                auto const & p1 = samples[connection[1]];
                float dist = p0.dist(p1);
                float avg_depth = (p0.z+p1.z)*0.5f;
                if(dist * threshold_factor <= avg_depth){
                    connections.push_back(connection);
                }
            }
            nextEdgeID = subdiv.nextEdge(nextEdgeID);
        } while(nextEdgeID != edgeID);
    }

    return connections;
}


template<typename computation_type_>
typename problem_kinect<computation_type_>::computation_type
problem_kinect<computation_type_>::computeResidual( point_3d const & p, plane_3d const & h ){
    if(p.is_outlier)
        return computation_type(2);
    return computation_type(h.dist(p));
}

template<typename computation_type_>
typename problem_kinect<computation_type_>::computation_type
problem_kinect<computation_type_>::getNoiseLevel(){
    return computation_type(0.05);
}

template<typename computation_type_>
typename problem_kinect<computation_type_>::computation_type
problem_kinect<computation_type_>::getNeighbourhoodWeight(){
    return computation_type(0.15);
}

template<typename computation_type_>
typename problem_kinect<computation_type_>::computation_type
problem_kinect<computation_type_>::getHighlevelPriorsWeight(){
    return computation_type(20);
}

template<typename computation_type_>
size_t
problem_kinect<computation_type_>::getExpectedNumberOfStructures(){
    return 4;
}

template<typename computation_type_>
size_t
problem_kinect<computation_type_>::getNumberOfParametersPerHypothesis(){
    // A 3d plane can be identified by 3 parameters
    return 3;
}

template<typename computation_type_>
typename problem_kinect<computation_type_>::computation_type
problem_kinect<computation_type_>::getHypothesisCost( plane_3d const & ){
    return computation_type(1);
}

template<typename computation_type_>
typename problem_kinect<computation_type_>::computation_type
problem_kinect<computation_type_>::getHypothesisInteractionCost( plane_3d const & p0,
                                              plane_3d const & p1 ){
    computation_type dot = computation_type(p0.nx * p1.nx + p0.ny * p1.ny + p0.nz * p1.nz);

    computation_type crossx = computation_type(p0.ny*p1.nz-p0.nz*p1.ny);
    computation_type crossy = computation_type(p0.nz*p1.nx-p0.nx*p1.nz);
    computation_type crossz = computation_type(p0.nx*p1.ny-p0.ny*p1.nx);
    computation_type cross2 = crossx*crossx + crossy*crossy + crossz*crossz;


    computation_type dot2 = dot*dot;

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
std::vector<plane_3d>
problem_kinect<computation_type_>::generateHypotheses( std::vector<point_3d> const & points,
                                    size_t num_hypotheses )
{

    const size_t NUM_NEAREST_NEIGHBOURS = 200;

    // initialize random gen
    std::mt19937 gen(12);
    auto rnd_int = std::uniform_int_distribution<size_t>(0,points.size()-1);
    auto rnd_int_nn = std::uniform_int_distribution<int>(0, NUM_NEAREST_NEIGHBOURS-1);

    // create hypotheses vector
    std::vector<plane_3d> hypotheses;
    hypotheses.reserve(num_hypotheses);

    while(hypotheses.size()<num_hypotheses){

        auto const & p0 = points[rnd_int(gen)];

        // Compute nearest neighbours
        std::multimap<float, const point_3d*> nearest_neighbours;
        for(auto const & p : points){
            if (p.equals(p0)) continue;
            nearest_neighbours.insert(std::pair<float,const point_3d*>(p.dist(p0), &p));
            while(nearest_neighbours.size() > NUM_NEAREST_NEIGHBOURS)
                nearest_neighbours.erase(--nearest_neighbours.end());
        }

        int pos1 = rnd_int_nn(gen);
        auto it1 = nearest_neighbours.begin();
        for(int i = 0; i < pos1; i++){
            it1++;
        }
        const point_3d* p1 = it1->second;

        int pos2 = rnd_int_nn(gen);
        auto it2 = nearest_neighbours.begin();
        for(int i = 0; i < pos2; i++){
            it2++;
        }
        const point_3d* p2 = it2->second;



        if( p0.is_outlier || p1->is_outlier || p2->is_outlier ){
            continue;
        }

        plane_3d hypothesis(p0,*p1,*p2);

        size_t num_inliers = 0;
        for(size_t i = 0; i < points.size(); i++){
            computation_type val = computeResidual(points[i],hypothesis);
            if(val< 0) val = -val;
            if(val < getNoiseLevel()){
                num_inliers++;
            }
        }

        if(num_inliers < 100)
            continue;

        hypotheses.push_back(std::move(hypothesis));

    }

    return hypotheses;
}

