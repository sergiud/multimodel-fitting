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

#include "../../examples/kinect/data_reader.hxx"
#include "../../examples/kinect/img_depth.hxx"

#include <opencv2/opencv.hpp>

#include <vector>

size_t data_reader::getWidth(){

    std::vector<unsigned char> img_depth_data(img_depth, img_depth + img_depth_len);
    cv::Mat img = cv::imdecode(cv::Mat(img_depth_data), -CV_LOAD_IMAGE_ANYDEPTH);
    return size_t(img.cols);

}

size_t data_reader::getHeight(){

    std::vector<unsigned char> img_depth_data(img_depth, img_depth + img_depth_len);
    cv::Mat img = cv::imdecode(cv::Mat(img_depth_data), -CV_LOAD_IMAGE_ANYDEPTH);
    return size_t(img.rows);

}

std::vector<point_3d> data_reader::get()
{

    std::vector<unsigned char> img_depth_data(img_depth, img_depth + img_depth_len);
    cv::Mat img = cv::imdecode(cv::Mat(img_depth_data), -CV_LOAD_IMAGE_ANYDEPTH);

    std::cout << "width: " << img.cols << std::endl;
    std::cout << "height: " << img.rows << std::endl;

    std::vector<point_3d> points;
    points.reserve(size_t(img.cols * img.rows));

    const float fx = 535.4f;
    const float fy = 539.2f;
    const float cx = 320.1f;
    const float cy = 247.6f;
    const float factor = 5000;

    for(int v = 0; v < img.rows; v++){
        for(int u = 0; u < img.cols; u++){
            uint16_t val = img.at<uint16_t>(v,u);
            float Z = float(val)/factor;
            float X = (u - cx) * Z / fx;
            float Y = (v - cy) * Z / fy;
            if(u%8 == 0 && v%8 == 0 && val != 0)
                points.push_back(point_3d(X,Y,Z,u,v,val==0));
        }
    }

    return points;
}
