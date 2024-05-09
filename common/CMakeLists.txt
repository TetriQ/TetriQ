# SPDX-FileCopyrightText: 2024 The TetriQ authors
#
# SPDX-License-Identifier: AGPL-3.0-or-later

file(GLOB_RECURSE COMMON_SRC CONFIGURE_DEPENDS "${TetriQ_SOURCE_DIR}/common/src/*.cpp")

add_library(tetriq_common ${COMMON_SRC})
target_include_directories(tetriq_common
    PUBLIC "${TetriQ_SOURCE_DIR}/common/include")
