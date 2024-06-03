// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "ClientConfig.hpp"
#include <cstdint>
#include <string>

tetriq::ClientConfig::ClientConfig(const std::string &config_name)
    : AConfig(config_name)
{
    max_incoming_bandwidth =
        _table["max_incoming_bandwidth"].value<int64_t>().value_or(this->max_incoming_bandwidth);
    max_outgoing_bandwidth =
        _table["max_outgoing_bandwidth"].value<int64_t>().value_or(this->max_outgoing_bandwidth);
    server_timeout = _table["server_timeout"].value<int64_t>().value_or(this->server_timeout);
}
