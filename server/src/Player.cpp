// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "Player.hpp"
#include "Logger.hpp"
#include <cstdint>
#include <string>

tetriq::Player::Player(uint64_t network_id)
    : _network_id(network_id)
{
    Logger::log(LogLevel::INFO, "player " + std::to_string(_network_id) + " connected.");
}

tetriq::Player::~Player()
{
    Logger::log(LogLevel::INFO, "player " + std::to_string(_network_id) + " disconnected.");
}

uint64_t tetriq::Player::getNetworkId() const
{
    return _network_id;
}
