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
  ../../examples/kinect/shapes_3d.hxx
  ../../examples/kinect/data_reader.hxx
  problem_kinect.hxx
  correct_result.hxx
  main_templated.hxx
)

set(SOURCES
  ../../examples/kinect/shapes_3d.cxx
  data_reader.cxx
)

set(IMAGE_DEPTH cabinet_depth.png)
set(IMAGE_RGB   cabinet_rgb.png)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_definitions( "-Wno-exit-time-destructors" )
endif()


add_custom_command(
    OUTPUT img_depth.hxx
    COMMAND python ${CMAKE_CURRENT_LIST_DIR}/../../examples/kinect/embed_resource.py
            img_depth ${CMAKE_CURRENT_LIST_DIR}/data/${IMAGE_DEPTH} img_depth.hxx
    DEPENDS data/${IMAGE_DEPTH} ../../examples/kinect/embed_resource.py
    VERBATIM
)

add_custom_command(
    OUTPUT img_rgb.hxx
    COMMAND python ${CMAKE_CURRENT_LIST_DIR}/../../examples/kinect/embed_resource.py
            img_rgb ${CMAKE_CURRENT_LIST_DIR}/data/${IMAGE_RGB} img_rgb.hxx
    DEPENDS data/${IMAGE_RGB} ../../examples/kinect/embed_resource.py
    VERBATIM
)

add_executable(${TEST_NAME}_double ${SOURCES} ${HEADERS} main_double.cxx img_depth.hxx img_rgb.hxx)
target_link_libraries(${TEST_NAME}_double ${OpenCV_LIBRARIES} mfigp)
target_include_directories (${TEST_NAME}_double PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
add_test(NAME ${TEST_NAME}_double COMMAND ${TEST_NAME}_double)

add_executable(${TEST_NAME}_float ${SOURCES} ${HEADERS} main_float.cxx img_depth.hxx img_rgb.hxx)
target_link_libraries(${TEST_NAME}_float ${OpenCV_LIBRARIES} mfigp)
target_include_directories (${TEST_NAME}_float PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
add_test(NAME ${TEST_NAME}_float COMMAND ${TEST_NAME}_float)
