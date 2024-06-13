// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once
#include <queue>
#include <string>
#include <vector>

namespace tetriq {
    class Server;
    class Channel;

#define ASSERTS_ARGS(args, res_queue) \
    if (args.empty()) { \
        LogLevel::ERROR << "No arguments provided" << std::endl; \
        res_queue.emplace("RCON: No arguments provided\n"); \
        return; \
    }

    class ServerManager {
            using _args = const std::vector<std::string> &;

        public:
            ServerManager(Server &server);
            ~ServerManager();

            void _startGame(_args, std::queue<std::string> &res_queue);
            void _stopGame(_args, std::queue<std::string> &res_queue);
            void _get(_args, std::queue<std::string> &res_queue);
            void _set(_args, std::queue<std::string> &res_queue);
            void _list(_args, std::queue<std::string> &res_queue);
            void _create(_args, std::queue<std::string> &res_queue);
            void _delete(_args, std::queue<std::string> &res_queue);

        private:
            Server &_server;
            Channel *_selected_channel{nullptr};
    };
}
