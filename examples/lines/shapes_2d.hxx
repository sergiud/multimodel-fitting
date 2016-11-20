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

struct point_2d {
public:
    float x;
    float y;
    point_2d(float x, float y);
    float dist(point_2d const & p) const;
};

struct line_2d {
public:
    float x0, y0;
    float dirx, diry;
    line_2d(point_2d const & p0, point_2d const & p1);
    point_2d get_point(float dist) const;
    float dist(point_2d const & p) const;
};
