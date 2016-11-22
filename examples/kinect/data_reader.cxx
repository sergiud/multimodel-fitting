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

#include "data_reader.hxx"

#include <opencv2/opencv.hpp>

#include <vector>

extern unsigned char img_depth[];
extern size_t img_depth_len;

std::vector<point_3d> data_reader::get()
{

    std::vector<unsigned char> img_depth_data(img_depth, img_rgb + img_depth_len);
    cv::Mat img = cv::imdecode(cv::Mat(img_depth_data), -CV_LOAD_IMAGE_ANYDEPTH);

    for(int i = 0; i < img.dims; i++){
        std::cout << img.size[i] << std::endl;
    }

    std::cout << "width: " << img.cols << std::endl;
    std::cout << "height: " << img.rows << std::endl;

    std::vector<point_3d> points;
    points.reserve(img.cols * img.rows);

    const double fx = 535.4;
    const double fy = 539.2;
    const double cx = 320.1;
    const double cy = 247.6;
    const double factor = 5000;

    for(int v = 0; v < img.rows; v++){
        for(int u = 0; u < img.cols; u++){
            double val = img.at<uint16_t>(v,u);
            double Z = val/factor;
            double X = (u - cx) * Z / fx;
            double Y = (v - cy) * Z / fy;
            points.push_back(point_3d(X,Y,Z));
        }
    }

    return points;
}
