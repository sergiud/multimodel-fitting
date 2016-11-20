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

float point_2d::dist(point_2d const & p) const
{
    float dx = p.x - x;
    float dy = p.y - y;
    return sqrtf(dx * dx + dy * dy);
}

line_2d::line_2d(point_2d const & p0, point_2d const & p1)
    : x0(p0.x), y0(p0.y),
    dirx((p1.x - p0.x) / sqrt((p1.x - p0.x)*(p1.x - p0.x) + (p1.y - p0.y)*(p1.y - p0.y))),
    diry((p1.y - p0.y) / sqrt((p1.x - p0.x)*(p1.x - p0.x) + (p1.y - p0.y)*(p1.y - p0.y)))
{
}

point_2d line_2d::get_point(float dist) const
{
    return point_2d(x0 + dist * dirx, y0 + dist * diry);
}

float line_2d::dist(point_2d const & p) const
{
    float dx = p.x - x0;
    float dy = p.y - y0;
    return dx * diry - dy * dirx;
}
