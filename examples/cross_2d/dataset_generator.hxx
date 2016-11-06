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

#include <cstddef>
#include <vector>
#include <random>
#include "shapes_2d.hxx"

class dataset_generator {
public:
    static std::vector<point_2d>generate(size_t num_data, size_t num_outliers);
    static std::vector<line_2d> compute_hypotheses(std::vector<point_2d> const & datapoints, size_t num);
private:
    static std::vector<point_2d>generate_data(size_t num);
    static std::vector<point_2d>generate_outliers(size_t num);
};
