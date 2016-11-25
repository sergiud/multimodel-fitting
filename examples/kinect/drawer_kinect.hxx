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

#include "shapes_3d.hxx"
#include <vector>
#include <array>
#include <random>
#include <opencv2/opencv.hpp>


class drawer_kinect {
private:
    drawer_kinect();
public:
    drawer_kinect(const char* name);
    void set_datapoints(std::vector<point_3d> const &);
    void set_neighbourhood(std::vector<std::array<size_t, 2>> const & neighbourhood);
    void set_hypothesis_count(size_t);
    void clear();
    void draw_empty();
    void draw_labeled(std::vector<int32_t> const & labels);
    void draw_connection(size_t id0, size_t id1);
    void display();
    void wait();
    void sleep(int milliseconds);
private:
    std::vector<std::array<float, 3>> colors;
    cv::Mat img_background;
    cv::Mat visu;
    const char* disp;
    std::mt19937 rnd_gen;
    std::uniform_real_distribution<float> col_hue_detail_gen;
    std::uniform_int_distribution<unsigned int> col_hue_raw_gen;
private:
    std::vector<point_3d> datapoints;
    std::vector<std::array<size_t, 2>> neighbourhood;
    std::array<float, 3> generate_color();

};

extern drawer_kinect drawer;
