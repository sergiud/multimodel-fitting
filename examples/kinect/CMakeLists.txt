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
  drawer_kinect.hxx
)

set(SOURCES
  main.cxx
  shapes_3d.cxx
  data_reader.cxx
  problem_kinect.cxx
  drawer_kinect.cxx
)

set(IMAGE_DEPTH cabinet_depth.png)
set(IMAGE_RGB   cabinet_rgb.png)

add_custom_command(
    OUTPUT img_depth.cxx
    COMMAND python ${CMAKE_CURRENT_LIST_DIR}/embed_resource.py img_depth
            ${CMAKE_CURRENT_LIST_DIR}/data/${IMAGE_DEPTH} img_depth.cxx
    DEPENDS data/${IMAGE_DEPTH} embed_resource.py
)

add_custom_command(
    OUTPUT img_rgb.cxx
    COMMAND python ${CMAKE_CURRENT_LIST_DIR}/embed_resource.py img_rgb
            ${CMAKE_CURRENT_LIST_DIR}/data/${IMAGE_RGB} img_rgb.cxx
    DEPENDS data/${IMAGE_RGB} embed_resource.py
)

add_executable(${EXAMPLE} ${SOURCES} ${HEADERS} img_depth.cxx img_rgb.cxx)
target_link_libraries(${EXAMPLE} ${OpenCV_LIBRARIES} mfigp)


