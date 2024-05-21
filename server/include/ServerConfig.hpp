// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "AConfig.hpp"
#include "GameConfig.hpp"
#include <cstdint>

namespace tetriq {
    class ServerConfig : AConfig {
        public:
            ServerConfig(const std::string &config_name = "server.toml");

            size_t max_clients = 128;
            uint32_t max_outgoing_bandwidth = 0;
            uint32_t max_incoming_bandwidth = 0;
            uint32_t client_timeout = 1000;
	    std::string listen_address = "0.0.0.0";
	    uint16_t listen_port = 31457;
            GameConfig game;
    };
}
