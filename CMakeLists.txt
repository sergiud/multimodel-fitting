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

# Add static dependencies
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

get_cmake_property(_variableNames VARIABLES)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()

# Add OpenGM Dependency
include(ExternalProject)
ExternalProject_Add( opengm
    PREFIX "${PROJECT_SOURCE_DIR}/external/opengm"
    GIT_REPOSITORY "https://github.com/opengm/opengm.git"
    #GIT_TAG "v2.3.7RC"
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${PROJECT_SOURCE_DIR}/external/install
               -DBUILD_EXAMPLES=OFF -DBUILD_TUTORIALS=OFF -DBUILD_TESTING=OFF
               -DWITH_MAXFLOW=ON
    INSTALL_DIR "${PROJECT_SOURCE_DIR}/external/install"
    PATCH_COMMAND eval "(cd ./src/external/patches/MaxFlow/ && ./patchMaxFlow-v3.02.sh)"
)
include_directories("${PROJECT_SOURCE_DIR}/external/opengm/src/opengm/src/external/MaxFlow-v3.02.src-patched")

# Add C++11
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
    CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
    if(COMPILER_SUPPORTS_CXX0X)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    else()
        message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11"
                            "support. Please use a different C++ compiler.")
    endif()
endif()

# Add main include directory
include_directories("${PROJECT_SOURCE_DIR}/include")

# Add dependency include directories
include_directories("${PROJECT_SOURCE_DIR}/external/install/include")
# TODO remove:
include_directories("${PROJECT_SOURCE_DIR}/ext/CImg")


add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(examples)
