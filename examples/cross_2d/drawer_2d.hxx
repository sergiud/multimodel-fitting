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

#include <Cimg.h>
#include "shapes_2d.hxx"

class drawer_2d {
private:
	drawer_2d();
public:
	drawer_2d(const char* name, unsigned int width, unsigned int height, float x_min, float x_max, float y_min, float y_max);
	void clear();
	void draw_line(line_2d line, unsigned char color[3]);
	void draw_point(point_2d point, unsigned char color[3]);
	void display();
	void wait();
private:
	cimg_library::CImg<unsigned char> visu;
	cimg_library::CImgDisplay disp;
	const float x_min, x_max, y_min, y_max;
	const unsigned int width, height;
};