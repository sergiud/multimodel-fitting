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

#include "shapes_2d.hxx"
#include <vector>
#include <array>
#include <random>
#include <opencv2/core/core.hpp>

class drawer_2d {
private:
    drawer_2d();
public:
    drawer_2d(const char* name, unsigned int width, unsigned int height, float x_min, float x_max, float y_min, float y_max);
    void clear();
    void draw_line(line_2d line, std::array<unsigned char,3> const & color);
    void draw_point(point_2d point, std::array<unsigned char, 3> const & color);
    void draw_connection(point_2d p0, point_2d p1, std::array<unsigned char, 3> const & color);
    void display();
    void wait();
public:
    void set_datapoints(std::vector<point_2d> const & points);
    void set_hypotheses(std::vector<line_2d> const & hypotheses);
    void draw_all();
    template<typename T> void draw_labeled(std::vector<T> const & labels);
private:
    std::vector<point_2d> datapoints;
    std::vector<line_2d> hypotheses;
    std::vector<std::array<unsigned char, 3>> colors_lines;
    std::vector<std::array<unsigned char, 3>> colors_dots;
    cv::Mat visu;
    const char* disp;
    const float x_min, x_max, y_min, y_max;
    const unsigned int width, height;
    std::mt19937 rnd_gen;
    std::uniform_real_distribution<float> col_hue_detail_gen;
    std::uniform_int_distribution<unsigned int> col_hue_raw_gen;
private:
    std::array<float, 3> generate_color();

};

template<typename T>
inline void drawer_2d::draw_labeled(std::vector<T> const & labels)
{
    std::vector<unsigned char> hypothesis_exists(hypotheses.size());
    std::fill(hypothesis_exists.begin(), hypothesis_exists.end(), false);
    for (size_t i = 0; i < labels.size(); i++) {
        hypothesis_exists[labels[i]] = true;
    }
    for (size_t i = 0; i < hypotheses.size(); i++) {
        if (hypothesis_exists[i]) {
            draw_line(hypotheses[i], colors_lines[i]);
        }
    }
    for (size_t i = 0; i < labels.size(); i++) {
        draw_point(datapoints[i], colors_dots[labels[i]]);
    }
}
