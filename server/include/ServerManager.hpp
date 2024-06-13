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

#define ASSERTS_ARGS(args) \
    if (args.empty()) { \
        LogLevel::ERROR << "No arguments provided" << std::endl; \
        return; \
    }

    class ServerManager {
            using _args = const std::vector<std::string> &;

        public:
            ServerManager(Server &server);
            ~ServerManager();

            void startGame(_args, std::queue<std::string> &res_queue);
            void get(_args, std::queue<std::string> &res_queue);
            void set(_args, std::queue<std::string> &res_queue);

        private:
            Server &_server;
            Channel *_selected_channel{nullptr};
    };
}
