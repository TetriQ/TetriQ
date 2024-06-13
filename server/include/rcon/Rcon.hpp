// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "RconConfig.hpp"
#include "rcon/CommandHandler.hpp"
#include "ServerManager.hpp"

#include <queue>

#define RCONLOG(level) LogLevel::level << "[Rcon] "

namespace tetriq {
    class Server;

    class RconClient {
        public:
            RconClient(int socket, sockaddr_in address);
            ~RconClient();
            int getSocket() const;
            RconClient &operator<<(const std::string &response);
            std::queue<std::string> _res_queue;
            bool _is_authenticated{false};

        private:
            int _socket;
            sockaddr_in _address;
    };

    class Rcon {
        public:
            void addCommands();
            Rcon(Server &server);
            ~Rcon();
            void listen();
            bool init();

            void registerCommand(const std::string &commandName,
                void (ServerManager::*func)(
                    const std::vector<std::string> &, std::queue<std::string> &));

        private:
            bool _is_running{false};
            Server &_server;
            RconConfig _config;
            int _rcon_socket;
            int _rcon_port;
            sockaddr_in _rcon_address;

            bool setHost();
            bool createHost();

            std::unique_ptr<RconClient> _client{nullptr};
            timeval _timeout{};
            fd_set _readfds{};
            fd_set _writefds{};

            void handleNewClient();
            void handleClient();

            ServerManager _server_manager{_server};
            command::CommandHandler _command_handler;
    };
}
