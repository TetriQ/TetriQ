// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "ClientConfig.hpp"
#include <cstdint>
#include <string>

tetriq::ClientConfig::ClientConfig(const std::string &config_name)
    : AConfig(config_name)
{
    max_incoming_bandwidth = _table["max_incoming_bandwidth"].value<int64_t>().value_or(0);
    max_outgoing_bandwidth = _table["max_outgoing_bandwidth"].value<int64_t>().value_or(0);
    server_timeout = _table["server_timeout"].value<int64_t>().value_or(1000);
}
