##
## This file is part of TetriQ.
## Copyright © 2024 Noé Lopez <noelopez@free.fr>
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

file(GLOB_RECURSE COMMON_SRC CONFIGURE_DEPENDS "${TetriQ_SOURCE_DIR}/common/src/*.cpp")

add_library(tetriq_common ${COMMON_SRC})
target_include_directories(tetriq_common
    PUBLIC "${TetriQ_SOURCE_DIR}/common/include")
