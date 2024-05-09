# SPDX-FileCopyrightText: 2024 The TetriQ authors
#
# SPDX-License-Identifier: AGPL-3.0-or-later

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
