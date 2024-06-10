# SPDX-FileCopyrightText: 2024 The TetriQ authors
#
# SPDX-License-Identifier: AGPL-3.0-or-later

file(GLOB_RECURSE SERVER_SRC CONFIGURE_DEPENDS "${TetriQ_SOURCE_DIR}/server/src/*.cpp")

find_package(ENet 1.3.17 REQUIRED)

add_executable(tetriq_server ${SERVER_SRC})

target_include_directories(tetriq_server
    PRIVATE "${TetriQ_SOURCE_DIR}/server/include"
)

target_link_libraries(tetriq_server
    PRIVATE tetriq_common
    PRIVATE enet
)

install(TARGETS tetriq_server DESTINATION bin)
install(FILES ../etc/server.toml DESTINATION etc/tetriq/)
