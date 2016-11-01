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

#include "drawer_2d.hxx"
#include <iostream>

drawer_2d::drawer_2d(const char * name, unsigned int width, unsigned int height, float x_min, float y_min, float x_max, float y_max)
	: width(width), height(height), x_min(x_min), x_max(x_max), y_min(y_min), y_max(y_max), visu(width, height,1,3,0), disp(visu, name)
{
}

void drawer_2d::clear()
{
	visu.fill(0);
}

void drawer_2d::draw_line(line_2d line, unsigned char color[3])
{
	point_2d p0 = line.get_point(static_cast<float>(width + height));
	point_2d p1 = line.get_point(-static_cast<float>(width + height));

	float p0_x_f = (p0.x - x_min) / (x_max - x_min);
	float p0_y_f = (p0.y - y_min) / (y_max - y_min);
	float p1_x_f = (p1.x - x_min) / (x_max - x_min);
	float p1_y_f = (p1.y - y_min) / (y_max - y_min);
	unsigned int p0_x = static_cast<unsigned int>(p0_x_f * width);
	unsigned int p0_y = static_cast<unsigned int>(p0_y_f * height);
	unsigned int p1_x = static_cast<unsigned int>(p1_x_f * width);
	unsigned int p1_y = static_cast<unsigned int>(p1_y_f * height);

	visu.draw_line(p0_x, p0_y, p1_x, p1_y, color);
}

void drawer_2d::draw_point(point_2d point, unsigned char color[3])
{
	float pos_x_f = (point.x - x_min) / (x_max - x_min);
	float pos_y_f = (point.y - y_min) / (y_max - y_min);
	unsigned int pos_x = static_cast<unsigned int>(pos_x_f * width);
	unsigned int pos_y = static_cast<unsigned int>(pos_y_f * height);
	visu.draw_rectangle(pos_x - 1, pos_y - 1, pos_x + 1, pos_y + 1, color);
}

void drawer_2d::display()
{
	visu.display(disp);
}

void drawer_2d::wait()
{
	while (!disp.is_closed()) {
		disp.wait();
	}
}
