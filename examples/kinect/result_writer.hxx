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

#include <vector>
#include <cstddef>
#include <cassert>
#include "shapes_3d.hxx"

class result_writer{
    public:
    template<typename label_type>
    static void write_stdout(std::vector<label_type> labels,
                             std::vector<point_3d> points);
};

template<typename label_type>
inline void result_writer::write_stdout( std::vector<label_type> labels,
                                         std::vector<point_3d> points ){
    assert(labels.size() == points.size());

    // Compute all existing labels
    std::set<label_type> active_labels(labels.begin(), labels.end());



    // Print all groups
    size_t next_label_id = 1;
    for(auto const & label : active_labels){
        size_t label_id;
        if(label == -1)
            label_id = 0;
        else
            label_id = next_label_id++;

        for(size_t i = 0; i < labels.size(); i++){
            if(labels[i] == label){
                std::cout << points[i].x << "," << points[i].y << ","
                          << points[i].z << "," << label_id << std::endl;
            }
        }

        label_id++;
    }


}
