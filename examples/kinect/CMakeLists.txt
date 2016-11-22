# Copyright (C) 2016  Martin Stumpf
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

set(HEADERS
  shapes_3d.hxx
  data_reader.hxx
  problem_kinect.hxx
)

set(SOURCES
  main.cxx
  shapes_3d.cxx
  data_reader.cxx
  problem_kinect.cxx
)

add_custom_command(
    OUTPUT img_depth.cxx
    COMMAND ${CMAKE_CURRENT_LIST_DIR}/embed_resource.py img_depth
            ${CMAKE_CURRENT_LIST_DIR}/data/cabinet_depth.png img_depth.cxx
    DEPENDS data/cabinet_depth.png embed_resource.py
)

add_custom_command(
    OUTPUT img_rgb.cxx
    COMMAND ${CMAKE_CURRENT_LIST_DIR}/embed_resource.py img_rgb
            ${CMAKE_CURRENT_LIST_DIR}/data/cabinet_rgb.png img_rgb.cxx
    DEPENDS data/cabinet_rgb.png embed_resource.py
)

add_executable(${EXAMPLE} ${SOURCES} ${HEADERS} img_depth.cxx img_rgb.cxx)
target_link_libraries(${EXAMPLE} ${OpenCV_LIBRARIES} mfigp)


