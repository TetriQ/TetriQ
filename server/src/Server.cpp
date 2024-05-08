/*
* This file is part of TetriQ.
* Copyright © 2024 Sébastien Lucas <sebastien.lucas@epitech.eu>
*
* TetriQ is free software: you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* TetriQ is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with TetriQ. If not, see <https://www.gnu.org/licenses/>.
*/

#include "Server.hpp"

#include <utility>

namespace tetriq {
    Server::Server(std::string ip, std::string port,
                const std::string &logfile_name)
        : _ip(std::move(ip)), _port(std::move(port)),
        _logger(logfile_name), _address(), _server(nullptr)
    {
        if (init() == false)
            throw ServerInitException();
    }

    bool Server::init()
    {
        _logger.log(LogLevel::INFO, "Server started");
        if (enet_initialize() != 0) {
            _logger.log(LogLevel::CRITICAL,
                "An error occurred while initializing ENet.");
            return false;
        }
        _logger.log(LogLevel::INFO, "ENet initialized");
        if (setHost() == false)
            return false;
        return true;
    }

    bool Server::setHost()
    {
        if (enet_address_set_host(&_address, _ip.c_str()) != 0) {
            const std::string message = "Failed to set host address: " + _ip;
            _logger.log(LogLevel::ERROR, message);
            return false;
        }
        try {
            _address.port = std::stoi(_port);
        } catch (const std::invalid_argument &e) {
            const std::string message = "Failed to set host port: " + _port;
            _logger.log(LogLevel::ERROR, message);
            return false;
        }
        const std::string message = "Host set to " + _ip + ":" + _port;
        _logger.log(LogLevel::INFO, message);
        return true;
    }

    Server::~Server()
    {
        enet_deinitialize();
        _logger.log(LogLevel::INFO, "ENet deinitialized");
        _logger.log(LogLevel::INFO, "Server stopped");
    }
}
