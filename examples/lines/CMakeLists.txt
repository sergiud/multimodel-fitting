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
  shapes_2d.hxx
  drawer_2d.hxx
  datapoints.hxx
  problem_lines.hxx
)

set(SOURCES
  main.cxx
  shapes_2d.cxx
  drawer_2d.cxx
  datapoints.cxx
  problem_lines.cxx
)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_definitions( "-Wno-exit-time-destructors" )
endif()

add_executable(${EXAMPLE_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${EXAMPLE_NAME} ${OpenCV_LIBRARIES} mfigp)
