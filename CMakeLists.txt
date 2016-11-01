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

cmake_minimum_required(VERSION 3.0)

project(multimodel_fitting)

set(multimodel_fitting_VERSION_MAJOR 0)
set(multimodel_fitting_VERSION_MINOR 1)


# Set output directories
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

# Add main include directory
include_directories("${PROJECT_SOURCE_DIR}/include")

# Add dependency include directories
include_directories("${PROJECT_SOURCE_DIR}/ext/CImg")

add_subdirectory(include)
message("multimodelfitting_HEADERS: ${multimodelfitting_HEADERS}")
add_subdirectory(src)
add_subdirectory(examples)