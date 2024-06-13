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

void tetriq::ServerManager::_startGame(
    const std::vector<std::string> &, std::queue<std::string> &res_queue)
{
    if (_selected_channel == nullptr) {
        res_queue.emplace("RCON: No channel selected\n");
        return;
    }
    _selected_channel->startGame();
    res_queue.emplace("RCON: Game started\n");
}

void tetriq::ServerManager::_stopGame(_args, std::queue<std::string> &res_queue)
{
    if (_selected_channel == nullptr) {
        res_queue.emplace("RCON: No channel selected\n");
        return;
    }
    _selected_channel->stopGame();
    res_queue.emplace("RCON: Game stopped\n");
}

void tetriq::ServerManager::_get(
    const std::vector<std::string> &args, std::queue<std::string> &res_queue)
{
    ASSERTS_ARGS(args, res_queue);
    res_queue.emplace("RCON: Get command not implemented\n");
    // TODO: Implement get command (get channel, get player....)
}

void tetriq::ServerManager::_set(
    const std::vector<std::string> &args, std::queue<std::string> &res_queue)
{
    ASSERTS_ARGS(args, res_queue);
    if (args[0] == "channel") {
        if (args.size() < 2) {
            res_queue.emplace("RCON: No channel id provided\n");
            return;
        }
        try {
            uint64_t channel_id = std::stoul(args[1]);
            if (channel_id >= _server.getChannels().size()) {
                throw std::out_of_range("Channel not found");
            }
            _selected_channel = &_server.getChannels()[channel_id];
            res_queue.emplace("RCON: Channel " + std::to_string(channel_id) + " selected\n");
            return;
        } catch (const std::invalid_argument &e) {
            res_queue.emplace("RCON: Invalid channel id\n");
        } catch (const std::out_of_range &e) {
            res_queue.emplace("RCON: Channel ID out of range\n");
        }
    } else {
        res_queue.emplace("RCON: 'set " + args[0] + "' command not implemented\n");
    }
}

void tetriq::ServerManager::_list(
    const std::vector<std::string> &args, std::queue<std::string> &res_queue)
{
    if (args.empty()) {
        res_queue.emplace("RCON: Channels:\n");
        for (auto &channel : _server.getChannels()) {
            res_queue.emplace("Channel " + std::to_string(channel.getChannelId()) + " - Players: "
                              + std::to_string(channel.getPlayers().size()) + " - Game started: "
                              + std::to_string(channel.hasGameStarted()) + "\n");
        }
        return;
    }
    res_queue.emplace("RCON: No arguments expected\n");
}

void tetriq::ServerManager::_create(
    const std::vector<std::string> &args, std::queue<std::string> &res_queue)
{
    ASSERTS_ARGS(args, res_queue);
    if (args[0] == "channel") {
        if (_server.createChannel()) {
            res_queue.emplace("RCON: Channel created\n");
        } else {
            res_queue.emplace("RCON: Channel creation failed\n");
        }
    } else {
        res_queue.emplace("RCON: 'create " + args[0] + "' command not implemented\n");
    }
}

void tetriq::ServerManager::_delete(_args, std::queue<std::string> &res_queue)
{
    if (_selected_channel == nullptr) {
        res_queue.emplace("RCON: No channel selected\n");
        return;
    }
    if (_server.deleteChannel(_selected_channel->getChannelId())) {
        res_queue.emplace("RCON: Channel deleted\n");
    } else {
        res_queue.emplace("RCON: Channel deletion failed\n");
    }
}
