# SPDX-FileCopyrightText: 2024 The TetriQ authors
#
# SPDX-License-Identifier: AGPL-3.0-or-later

##
## This file is part of TetriQ.
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

cmake_minimum_required(VERSION 3.12)

project(TetriQ LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 20)
add_compile_options("-Wall" "-Wextra")

add_subdirectory(common)
add_subdirectory(client)
add_subdirectory(server)

add_custom_target(TetriQ ALL
    DEPENDS tetriq_client tetriq_server)
