# SPDX-FileCopyrightText: 2024 The TetriQ authors
#
# SPDX-License-Identifier: AGPL-3.0-or-later

file(GLOB_RECURSE CLIENT_SRC CONFIGURE_DEPENDS "${TetriQ_SOURCE_DIR}/client/src/*.cpp")

find_package(SFML 2.5 REQUIRED COMPONENTS graphics)
find_package(Curses REQUIRED)
find_package(tomlplusplus REQUIRED)
find_package(ENet 1.3.17 REQUIRED)

add_executable(tetriq_client ${CLIENT_SRC})

target_include_directories(tetriq_client
    PRIVATE "${TetriQ_SOURCE_DIR}/client/include"
)

target_link_libraries(tetriq_client
    PRIVATE tetriq_common
    PRIVATE sfml-graphics
    PRIVATE ncurses
    PRIVATE enet
)
