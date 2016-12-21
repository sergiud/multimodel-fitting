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

project(mfigp)

set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)

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

enable_testing()
add_subdirectory(tests)

# Add Doxygen if available
find_package(Doxygen)
option(BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)" ${DOXYGEN_FOUND})

if(BUILD_DOCUMENTATION)
    if(NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "Doxygen is needed to build the documentation.")
    endif()

    set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/docs/Doxyfile)

    string (REPLACE ";" " " DOCUMENTED_FILES_STR "${DOCUMENTED_FILES}")

    configure_file(${doxyfile_in} ${doxyfile} @ONLY)

    add_custom_command(
        OUTPUT docs/doxygen.stamp
        DEPENDS ${doxyfile}
        COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
        COMMAND cmake -E touch docs/doxygen.stamp
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)
    add_custom_target(
        docs ALL
        DEPENDS docs/doxygen.stamp
    )

    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs/html DESTINATION share/doc)
endif()



