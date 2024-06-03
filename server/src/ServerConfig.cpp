// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "ServerConfig.hpp"
#include "GameConfig.hpp"

tetriq::ServerConfig::ServerConfig(const std::string &config_name)
    : AConfig(config_name)
{
    max_clients = _table["max_clients"].value<int64_t>().value_or(this->max_clients);
    max_incoming_bandwidth =
        _table["max_incoming_bandwidth"].value<int64_t>().value_or(this->max_incoming_bandwidth);
    max_outgoing_bandwidth =
        _table["max_outgoing_bandwidth"].value<int64_t>().value_or(this->max_outgoing_bandwidth);
    client_timeout = _table["client_timeout"].value<int64_t>().value_or(this->client_timeout);
    listen_address = _table["listen_address"].value_or(this->listen_address);
    listen_port = _table["listen_port"].value<int64_t>().value_or(this->listen_port);
    ticks_per_second = _table["ticks_per_second"].value<int64_t>().value_or(this->ticks_per_second);
    if (_table["game"].is_table())
        game = GameConfig{*_table["game"].as_table()};
}
