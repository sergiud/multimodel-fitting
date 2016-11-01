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
  problem_ortholines.hxx
  shapes_2d.hxx
  drawer_2d.hxx
  dataset_generator.hxx
)

set(SOURCES
  problem_ortholines.cxx
  shapes_2d.cxx
  drawer_2d.cxx
  dataset_generator.cxx
  main.cxx
)

add_executable(${EXAMPLE} ${SOURCES} ${HEADERS})
target_link_libraries(${EXAMPLE} multimodelfitting)
