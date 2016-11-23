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

#include "problem_kinect.hxx"
#include <iostream>
#include <cassert>

#include <opencv2/opencv.hpp>
#include "drawer_kinect.hxx"

void
problem_kinect::debug_output( std::vector<label_type> const & labels,
                              computation_type value ){
    std::cout << "Value: " << value << std::endl;
    std::vector<label_type> transformed_labels(labels.size());
    std::transform(labels.begin(), labels.end(), transformed_labels.begin(),
                   [](label_type x){return x-1;});
    drawer.draw_labeled(transformed_labels);
    drawer.sleep(1);
}

extern unsigned char img_depth[];
extern size_t img_depth_len;

std::vector<std::array<problem_kinect::sampleid_type,2>>
problem_kinect::computeNeighbourhood( std::vector<point_3d> const & points ){

    std::vector<unsigned char> img_depth_data(img_depth, img_depth + img_depth_len);
    cv::Mat img = cv::imdecode(cv::Mat(img_depth_data), -CV_LOAD_IMAGE_ANYDEPTH);

    size_t width = img.cols;
    size_t height = img.rows;

    assert( points.size() == width * height);

    const float threshold_factor = 10;

    std::vector<std::array<sampleid_type,2>> neighbours;
    for(int v = 0; v < height; v++){
        for(int u = 0; u < width-1; u++){
            size_t p0 = v*width + u;
            size_t p1 = p0 + 1;
            if(points[p0].z == 0 || points[p1].z == 0)
                continue;
            float delta_depth = points[p0].z-points[p1].z;
            if(delta_depth < 0) delta_depth = -delta_depth;
            float avg_depth = (points[p0].z+points[p1].z)/2.0f;
            if(delta_depth * threshold_factor > avg_depth) continue;
            std::array<sampleid_type,2> conn = {{p0,p1}};
            neighbours.push_back(std::move(conn));
        }
    }
    for(int v = 0; v < height-1; v++){
        for(int u = 0; u < width; u++){
            size_t p0 = v*width + u;
            size_t p1 = p0 + width;
            if(points[p0].z == 0 || points[p1].z == 0)
                continue;
            float delta_depth = points[p0].z-points[p1].z;
            if(delta_depth < 0) delta_depth = -delta_depth;
            float avg_depth = (points[p0].z+points[p1].z)/2.0f;
            if(delta_depth * threshold_factor > avg_depth) continue;
            std::array<sampleid_type,2> conn = {{p0,p1}};
            neighbours.push_back(std::move(conn));
        }
    }
    return neighbours;
}

double
problem_kinect::computeResidual( point_3d const & p, plane_3d const & h ){
    if(p.z == 0)
        return 2;
    return h.dist(p);
}

double
problem_kinect::getNoiseLevel(){
    // TODO
    return 0.02;
}

double
problem_kinect::getNeighbourhoodWeight(){
    // TODO
    return 0.1;
}

double
problem_kinect::getHighlevelPriorsWeight(){
    // TODO
    return 0;
}

size_t
problem_kinect::getExpectedNumberOfStructures(){
    return 10;
}

size_t
problem_kinect::getNumberOfParametersPerHypothesis(){
    // A 3d plane can be identified by 3 parameters
    return 3;
}

double
problem_kinect::getHypothesisCost( plane_3d const & ){
    // TODO
    return 0;
}

double
problem_kinect::getHypothesisInteractionCost( plane_3d const &,
                                              plane_3d const & ){
    // TODO
    return 0;
}

std::vector<plane_3d>
problem_kinect::generateHypotheses( std::vector<point_3d> const & points,
                                    size_t num_hypotheses)
{
    // initialize random gen
    std::random_device rd;
    std::mt19937 gen(5);//rd());
    auto rnd_int = std::uniform_int_distribution<size_t>(0,points.size()-1);

    // create hypotheses vector
    std::vector<plane_3d> hypotheses;
    hypotheses.reserve(num_hypotheses);

    while(hypotheses.size()<num_hypotheses){

        auto const & p0 = points[rnd_int(gen)];
        auto const & p1 = points[rnd_int(gen)];
        auto const & p2 = points[rnd_int(gen)];

        if(p0.z == 0 || p1.z == 0 || p2.z == 0){
            continue;
        }

        plane_3d hypothesis(p0,p1,p2);

        size_t num_inliers = 0;
        for(size_t i = 0; i < points.size(); i++){
            double val = computeResidual(points[i],hypothesis);
            if(val< 0) val = -val;
            if(val < getNoiseLevel()){
                num_inliers++;
            }
        }

        std::cout << "num_inliers: " << num_inliers << std::endl;

        if(num_inliers < 15000)
            continue;

        hypotheses.push_back(std::move(hypothesis));

    }

    return hypotheses;
}

