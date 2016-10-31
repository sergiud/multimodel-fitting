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

#include "config_ortholines.hxx"
#include <iostream>

point_2d::point_2d(float x, float y): x(x), y(y)
{
}

line_2d::line_2d(float angle, float dist): angle(angle), dist(dist)
{
}

void config_ortholines::debug_output(std::vector<MultiModelFitter_impl::label_type> const &)
{
	std::cout << "debug_output()" << std::endl;
}
