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

file(GLOB_RECURSE CLIENT_SRC CONFIGURE_DEPENDS "${TetriQ_SOURCE_DIR}/client/src/*.cpp")

find_package(SFML 2.5 REQUIRED COMPONENTS graphics)
find_package(Curses REQUIRED)
find_package(ENet REQUIRED)
find_package(tomlplusplus REQUIRED)

add_executable(tetriq_client ${CLIENT_SRC})

target_include_directories(tetriq_client
    PRIVATE "${TetriQ_SOURCE_DIR}/client/include"
    PRIVATE ${CURSES_INCLUDE_DIRS}
    PRIVATE ${ENET_INCLUDE_DIRS}
)

target_link_libraries(tetriq_client
    PRIVATE tetriq_common
    PRIVATE sfml-graphics
    PRIVATE ${CURSES_LIBRARIES}
    PRIVATE ${ENET_LIBRARIES}
)
