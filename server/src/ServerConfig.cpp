// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "ServerConfig.hpp"
#include "GameConfig.hpp"

tetriq::ServerConfig::ServerConfig(const std::string &config_name)
    : AConfig(config_name)
{
    max_clients = _table["max_clients"].value<int64_t>().value_or(128);
    max_incoming_bandwidth = _table["max_incoming_bandwidth"].value<int64_t>().value_or(0);
    max_outgoing_bandwidth = _table["max_outgoing_bandwidth"].value<int64_t>().value_or(0);
    client_timeout = _table["client_timeout"].value<int64_t>().value_or(1000);
    listen_address = _table["listen_address"].value_or("0.0.0.0");
    listen_port = _table["listen_port"].value<int64_t>().value_or(31457);
    ticks_per_second = _table["ticks_per_second"].value<int64_t>().value_or(60);
    if (_table["game"].is_table())
        game = GameConfig{*_table["game"].as_table()};
}
