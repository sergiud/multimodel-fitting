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

#include "shapes_3d.hxx"
#include <cmath>

point_3d::point_3d(float x, float y, float z) : x(x), y(y), z(z)
{
}

float point_3d::dist(point_3d const & p) const
{
    float dx = p.x - x;
    float dy = p.y - y;
    float dz = p.z - z;
    return sqrtf(dx * dx + dy * dy + dz * dz);
}

