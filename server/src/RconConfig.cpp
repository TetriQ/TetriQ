// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "RconConfig.hpp"

tetriq::RconConfig::RconConfig(toml::table &config)
{
    enabled = config["enabled"].value<bool>().value_or(this->enabled);
    password = config["password"].value_or(this->password);
    listen_address = config["listen_address"].value_or(this->listen_address);
    listen_port = config["listen_port"].value<uint16_t>().value_or(this->listen_port);
}
