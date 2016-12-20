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

cmake_minimum_required(VERSION 3.1)

project(multimodel_fitting)

set(multimodel_fitting_VERSION_MAJOR 0)
set(multimodel_fitting_VERSION_MINOR 1)

# Set output directories
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

# Add OpenCV dependency
find_package(OpenCV REQUIRED core highgui calib3d imgproc)
include_directories("${OpenCV_INCLUDE_DIRS}")

# Add C++11
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

# Set global compiler flags
if(NOT MSVC)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        add_definitions( "-Weverything -Wno-padded -Wno-non-virtual-dtor
                          -Wno-c++98-compat -Wno-cast-align" )
    else()
        add_definitions( "-Wall" ) # -Wpedantic -Wsign-conversion" )
    endif()
endif()

# Add main include directory
include_directories("${PROJECT_SOURCE_DIR}/include")

add_subdirectory(include)
add_subdirectory(examples)

