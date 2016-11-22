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
    void clear();
    void draw_empty();
    void draw_labeled(std::vector<uint32_t> const & labels);
    void draw_connection(size_t id0, size_t id1);
    void display();
    void wait();
    void sleep(int milliseconds);
private:
    std::vector<std::array<float, 3>> colors;
    unsigned int width, height;
    cv::Mat img_background;
    cv::Mat visu;
    const char* disp;
    std::mt19937 rnd_gen;
    std::uniform_real_distribution<float> col_hue_detail_gen;
    std::uniform_int_distribution<unsigned int> col_hue_raw_gen;
private:
    std::array<float, 3> generate_color();
    size_t getX(size_t id);
    size_t getY(size_t id);

};

extern drawer_kinect drawer;
