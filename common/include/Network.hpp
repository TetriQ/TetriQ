// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <enet/enet.h>
#include <iostream>

namespace tetriq {
    const std::string ENET_INIT_ERROR =
        "An error occurred while initializing ENet.";
    const std::string ENET_INIT_SUCCESS = "ENet initialized successfully.";
    const std::string ENET_DEINIT_SUCCESS = "ENet deinitialized successfully.";

    class NetworkObject {
        public:
            virtual ~NetworkObject() = default;
            // TODO
    };
}
