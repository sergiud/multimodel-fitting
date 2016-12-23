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
  ../../examples/lines/shapes_2d.hxx
  ../../examples/lines/datapoints.hxx
  problem_lines.hxx
  correct_result.hxx
  main_templated.hxx
)

set(SOURCES
  ../../examples/lines/shapes_2d.cxx
  ../../examples/lines/datapoints.cxx
)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_definitions( "-Wno-exit-time-destructors" )
endif()

add_executable(${TEST_NAME}_double ${SOURCES} ${HEADERS} main_double.cxx)
target_link_libraries(${TEST_NAME}_double ${OpenCV_LIBRARIES} mfigp)
add_test(NAME ${TEST_NAME}_double COMMAND ${TEST_NAME}_double)

add_executable(${TEST_NAME}_float ${SOURCES} ${HEADERS} main_float.cxx)
target_link_libraries(${TEST_NAME}_float ${OpenCV_LIBRARIES} mfigp)
add_test(NAME ${TEST_NAME}_float COMMAND ${TEST_NAME}_float)
