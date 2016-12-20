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

set(multimodelfitting_HEADERS

  "${CMAKE_CURRENT_SOURCE_DIR}/mfigp.hxx"
  "${CMAKE_CURRENT_SOURCE_DIR}/mfigp/alphaexpansionfitter.hxx"
  "${CMAKE_CURRENT_SOURCE_DIR}/mfigp/config.hxx"
  "${CMAKE_CURRENT_SOURCE_DIR}/mfigp/mincut_maxflow.hxx"
  "${CMAKE_CURRENT_SOURCE_DIR}/mfigp/ext/block.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/mfigp/ext/graph.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/mfigp/ext/graph.txx"

)

set(DOCUMENTED_FILES ${DOCUMENTED_FILES}
  "${CMAKE_CURRENT_SOURCE_DIR}/mfigp.hxx"
  "${CMAKE_CURRENT_SOURCE_DIR}/mfigp/config.hxx"
  PARENT_SCOPE
)


add_library(mfigp INTERFACE)
target_sources(mfigp INTERFACE ${multimodelfitting_HEADERS})
