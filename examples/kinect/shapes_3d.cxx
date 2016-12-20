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

#include <iostream>

point_3d::point_3d( float x_, float y_, float z_, int u_, int v_, bool is_outlier_ )
    : x(x_), y(y_), z(z_), u(u_), v(v_), is_outlier(is_outlier_)
{
}

float point_3d::dist(point_3d const & p) const
{
    float dx = p.x - x;
    float dy = p.y - y;
    float dz = p.z - z;
    return sqrtf(dx * dx + dy * dy + dz * dz);
}

bool point_3d::equals(point_3d const &p) const
{
    if(p.u == u && p.v == v) return true;
    return false;
}

plane_3d::plane_3d(point_3d const & p0, point_3d const & p1, point_3d const & p2){

    x0 = p0.x;
    y0 = p0.y;
    z0 = p0.z;

    float dx0 = p1.x-p0.x;
    float dy0 = p1.y-p0.y;
    float dz0 = p1.z-p0.z;
    float dx1 = p2.x-p0.x;
    float dy1 = p2.y-p0.y;
    float dz1 = p2.z-p0.z;

    nx = dy0*dz1-dz0*dy1;
    ny = dz0*dx1-dx0*dz1;
    nz = dx0*dy1-dy0*dx1;
/*
    std::cout << "(" << dx0 << "," << dy0 << "," << dz0
            << ")x(" << dx1 << "," << dy1 << "," << dz1
            << ")=(" << nx << "," << ny << "," << nz << ")" << std::endl;
*/
    float abs = sqrtf(nx*nx+ny*ny+nz*nz);
    nx = nx/abs;
    ny = ny/abs;
    nz = nz/abs;

}

float plane_3d::dist(point_3d const & p) const{

    float dx = p.x - x0;
    float dy = p.y - y0;
    float dz = p.z - z0;
    return dx*nx + dy*ny + dz*nz;

}
