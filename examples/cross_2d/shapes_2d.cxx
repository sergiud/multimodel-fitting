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

#include "shapes_2d.hxx"
#include <cmath>

point_2d::point_2d(float nx, float ny) : x(nx), y(ny)
{
}

line_2d::line_2d(float x0, float y0, float x1, float y1)
    : x0(x0), y0(y0),
    dirx((x1 - x0) / sqrt((x1 - x0)*(x1 - x0) + (y1 - y0)*(y1 - y0))),
    diry((y1 - y0) / sqrt((x1 - x0)*(x1 - x0) + (y1 - y0)*(y1 - y0)))
{
}

point_2d line_2d::get_point(float dist)
{
    return point_2d(x0 + dist * dirx, y0 + dist * diry);
}
