// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "RconConfig.hpp"

namespace tetriq {
    class Server;

    class RconClient {
        public:
            RconClient(int socket, sockaddr_in address);
            ~RconClient();
            int getSocket() const;

        private:
            bool _is_authenticated{false};
            int _socket;
            sockaddr_in _address;
    };

    class Rcon {
        public:
            Rcon(Server &server);
            ~Rcon();
            void listen();
            bool init();

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

            // std::vector<int> _connections;
    };
}
