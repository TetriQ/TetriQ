// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "ServerManager.hpp"

#include <Logger.hpp>
#include <iostream>
#include "Channel.hpp"

#include <Server.hpp>

tetriq::ServerManager::ServerManager(Server &server)
    : _server(server)
{}

tetriq::ServerManager::~ServerManager() = default;

void tetriq::ServerManager::startGame(
    const std::vector<std::string> &, std::queue<std::string> &res_queue)
{
    if (_selected_channel == nullptr) {
        res_queue.emplace("RCON: No channel selected\n");
        return;
    }
    _selected_channel->startGame();
    res_queue.emplace("RCON: Game started\n");
}

void tetriq::ServerManager::get(const std::vector<std::string> &args, std::queue<std::string> &)
{
    ASSERTS_ARGS(args);
    // TODO: Implement get command (get channel, get player....)
}

void tetriq::ServerManager::set(
    const std::vector<std::string> &args, std::queue<std::string> &res_queue)
{
    ASSERTS_ARGS(args);
    if (args[0] == "channel") {
        if (args.size() < 2) {
            res_queue.emplace("RCON: No channel id provided\n");
            return;
        }
        try {
            if (std::stoul(args[1]) >= _server.getChannels().size()) {
                throw std::out_of_range("Channel not found");
            }
            _selected_channel = &_server.getChannels()[std::stoul(args[1])];
        } catch (const std::invalid_argument &e) {
            res_queue.emplace("RCON: Invalid channel id\n");
        } catch (const std::out_of_range &e) {
            res_queue.emplace("RCON: Channel ID out of range\n");
        }
    }
}
