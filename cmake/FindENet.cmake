# SPDX-FileCopyrightText: 2024 The TetriQ authors
#
# SPDX-License-Identifier: AGPL-3.0-or-later

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
