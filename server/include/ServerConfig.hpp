// SPDX-License-Identifier: AGPL-3.0-or-later

#include "AConfig.hpp"
#include <cstdint>

namespace tetriq {
    class ServerConfig : AConfig {
        public:
            ServerConfig(const std::string &config_name = "server.toml");

            size_t max_clients = 5;
            uint32_t max_outgoing_bandwidth = 0;
            uint32_t max_incoming_bandwidth = 0;
            uint32_t client_timeout = 1000;
    };
}
