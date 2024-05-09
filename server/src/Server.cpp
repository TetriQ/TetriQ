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
        std::ostream &out, std::ostream &err)
        : _ip(std::move(ip)), _port(std::move(port)),
        _logger(out, err), _address(), _server(nullptr)
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
        if (not setHost() or not createHost())
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

    bool Server::createHost()
    {
        _server = enet_host_create(&_address, _max_clients, _max_channels,
            _max_incoming_bandwidth, _max_outgoing_bandwidth);
        if (_server == nullptr) {
            _logger.log(LogLevel::CRITICAL,
                "An error occurred while creating the server.");
            return false;
        }
        const std::string message =
            "Server created with max clients: " + std::to_string(_max_clients)
            + ", max channels: "
            + std::to_string(_max_channels)
            + ", max incoming bandwidth: "
            + std::to_string(_max_incoming_bandwidth)
            + ", max outgoing bandwidth: "
            + std::to_string(_max_outgoing_bandwidth);
        _logger.log(LogLevel::DEBUG, message);
        return true;
    }

    void Server::listen()
    {
        ENetEvent event;
        while (not should_exit && _running
            && enet_host_service(_server, &event, _timeout) >= 0) {
            switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                    handleNewClient(event);
                    break;
                case ENET_EVENT_TYPE_DISCONNECT:
                    handleClientDisconnect(event);
                    break;
                case ENET_EVENT_TYPE_RECEIVE:
                    handleClientPacket(event);
                    enet_packet_destroy(event.packet);
                    break;
                case ENET_EVENT_TYPE_NONE:
                    handleNone(event);
                    break;
            }
        }
        if (should_exit) {
            std::string message = "SIGINT received,stopping server";
            _logger.log(LogLevel::INFO, message);
        }
    }

    bool Server::handleNewClient(ENetEvent &event)
    {
        std::string message = "New client connected from ";
        message += std::to_string(event.peer->address.host);
        message += ":";
        message += std::to_string(event.peer->address.port);
        _logger.log(LogLevel::INFO, message);
        return true;
    }

    void Server::handleClientDisconnect(ENetEvent &event)
    {
        std::string message = "Client disconnected from ";
        message += std::to_string(event.peer->address.host);
        message += ":";
        message += std::to_string(event.peer->address.port);
        _logger.log(LogLevel::INFO, message);
        event.peer->data = nullptr;
    }

    void Server::handleClientPacket(ENetEvent &event)
    {
        std::string message = "Packet received from ";
        message += std::to_string(event.peer->address.host);
        message += ":";
        message += std::to_string(event.peer->address.port);
        _logger.log(LogLevel::INFO, message);
    }

    void Server::handleNone(ENetEvent &event) const
    {
        _logger.log(LogLevel::DEBUG, "No event occurred");
    }

    Server::~Server()
    {
        enet_host_destroy(_server);
        enet_deinitialize();
        _logger.log(LogLevel::INFO, "ENet deinitialized");
        _logger.log(LogLevel::INFO, "Server stopped");
    }
}
