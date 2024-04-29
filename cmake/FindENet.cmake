##
## This file is part of TetriQ.
## Copyright © 2024 Sébastien Lucas <sebastien.lucas@epitech.eu>
##
## TetriQ is free software: you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## TetriQ is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
## General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with TetriQ. If not, see <https://www.gnu.org/licenses/>.
##

# ENET_FOUND - system has ENet
# ENET_INCLUDE_DIRS - the ENet include directories
# ENET_LIBRARIES - link these to use ENet

find_path(ENET_INCLUDE_DIR enet/enet.h)
find_library(ENET_LIBRARY NAMES enet)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ENet DEFAULT_MSG ENET_LIBRARY ENET_INCLUDE_DIR)

if(ENET_FOUND)
    set(ENET_INCLUDE_DIRS ${ENET_INCLUDE_DIR})
    set(ENET_LIBRARIES ${ENET_LIBRARY})
else()
    set(ENET_LIBRARIES)
endif()

mark_as_advanced(ENET_INCLUDE_DIR ENET_LIBRARY)