// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "Rcon.hpp"

#include <Logger.hpp>
#include <Server.hpp>

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
    /*TODO: Implement Rcon listen*/
    // accept new connections
    // read data from connections
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
