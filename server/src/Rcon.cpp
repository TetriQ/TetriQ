// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "Rcon.hpp"

#include <Logger.hpp>
#include <Server.hpp>

tetriq::RconClient::RconClient(int socket, sockaddr_in address)
    : _socket(socket)
    , _address(address)
{}

tetriq::RconClient::~RconClient()
{
    close(_socket);
}

int tetriq::RconClient::getSocket() const
{
    return _socket;
}

tetriq::Rcon::Rcon(Server &server)
    : _server(server)
    , _config(_server.getConfig().rcon)
    , _rcon_socket()
    , _rcon_port()
    , _rcon_address()
{
    if (_config.enabled == false) {
        LogLevel::INFO << "Rcon disabled" << std::endl;
        return;
    }
    _readfds = {};
    LogLevel::INFO << "Rcon enabled" << std::endl;
}

tetriq::Rcon::~Rcon()
{
    if (_config.enabled == false) {
        return;
    }
    _is_running = false;
    close(_rcon_socket);
    LogLevel::INFO << "Rcon destroyed" << std::endl;
}

void tetriq::Rcon::listen()
{
    if (_is_running == false) {
        return;
    }
    FD_ZERO(&_readfds);
    FD_SET(_rcon_socket, &_readfds);
    if (_client != nullptr) {
        FD_SET(_client->getSocket(), &_readfds);
    }
    int activity = select(FD_SETSIZE, &_readfds, nullptr, nullptr, &_timeout);
    if (activity < 0) {
        LogLevel::ERROR << "[RCON] Select error" << std::endl;
        return;
    }
    if (FD_ISSET(_rcon_socket, &_readfds)) {
        sockaddr_in new_client_addr{};
        socklen_t addrlen = sizeof(new_client_addr);
        int new_client_socket =
            accept(_rcon_socket, (struct sockaddr *) &new_client_addr, &addrlen);
        if (new_client_socket < 0) {
            LogLevel::ERROR << "[RCON] Accept error" << std::endl;
            return;
        }
        if (_client != nullptr) {
            std::string msg = "Maximum connections reached";
            send(new_client_socket, msg.c_str(), msg.size(), 0);
            close(new_client_socket);
            return;
        }
        _client = std::make_unique<RconClient>(new_client_socket, new_client_addr);
        LogLevel::INFO << "[RCON] New connection" << std::endl;
    }
    if (_client != nullptr and FD_ISSET(_client->getSocket(), &_readfds)) {
        char buffer[1024] = {0};
        const long int valread = recv(_client->getSocket(), buffer, 1024, 0);
        if (valread == 0) {
            close(_client->getSocket());
            _client = nullptr;
            LogLevel::INFO << "[RCON] Connection closed" << std::endl;
            return;
        }
        if (valread < 0) {
            close(_client->getSocket());
            _client = nullptr;
            LogLevel::ERROR << "[RCON] Read error" << std::endl;
            return;
        }
        LogLevel::INFO << "[RCON] Received: " << buffer;
    }
    /*TODO: Implement Rcon listen*/
    // handle commands
}

bool tetriq::Rcon::init()
{
    if (_config.enabled == false) {
        return false;
    }
    if (not setHost() or not createHost()) {
        return false;
    }
    _is_running = true;
    return true;
}

bool tetriq::Rcon::setHost()
{
    if (not _config.enabled) {
        return false;
    }
    _rcon_port = _config.listen_port;
    _rcon_address.sin_family = AF_INET;
    _rcon_address.sin_addr.s_addr = inet_addr(_config.listen_address.c_str());
    _rcon_address.sin_port = htons(_rcon_port);
    LogLevel::INFO << "[RCON] Host set to " << _config.listen_address << ":" << _config.listen_port
                   << std::endl;
    return true;
}

bool tetriq::Rcon::createHost()
{
    if (not _config.enabled) {
        return false;
    }
    _rcon_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_rcon_socket == -1) {
        LogLevel::ERROR << "[RCON] Failed to create socket" << std::endl;
        return false;
    }
    int opt = 1;
    setsockopt(
        _rcon_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT | SO_KEEPALIVE, &opt, sizeof(opt));
    if (bind(_rcon_socket, (struct sockaddr *) &_rcon_address, sizeof(_rcon_address)) < 0) {
        LogLevel::ERROR << "[RCON] Failed to bind socket" << std::endl;
        return false;
    }
    if (::listen(_rcon_socket, 1) < 0) {
        LogLevel::ERROR << "[RCON] Failed to listen" << std::endl;
        return false;
    }
    LogLevel::INFO << "[RCON] Server listening on " << _config.listen_address << ":"
                   << _config.listen_port << std::endl;
    return true;
}
