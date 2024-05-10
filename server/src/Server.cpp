// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "Server.hpp"
#include <utility>

namespace tetriq {
    Server::Server(std::string ip, std::string port)
        : _ip(std::move(ip))
        , _port(std::move(port))
        , _address()
        , _server(nullptr)
    {
        if (init() == false)
            throw ServerInitException();
    }

    bool Server::init()
    {
        Logger::log(LogLevel::INFO, "Server started");
        if (enet_initialize() != 0) {
            Logger::log(LogLevel::CRITICAL, ENET_INIT_ERROR);
            return false;
        }
        Logger::log(LogLevel::INFO, ENET_INIT_SUCCESS);
        if (not setHost() or not createHost())
            return false;
        return true;
    }

    bool Server::setHost()
    {
        if (enet_address_set_host(&_address, _ip.c_str()) != 0) {
            const std::string message = "Failed to set host address: " + _ip;
            Logger::log(LogLevel::ERROR, message);
            return false;
        }
        try {
            _address.port = std::stoi(_port);
        } catch (const std::invalid_argument &e) {
            const std::string message = "Failed to set host port: " + _port;
            Logger::log(LogLevel::ERROR, message);
            return false;
        }
        const std::string message = "Host set to " + _ip + ":" + _port;
        Logger::log(LogLevel::INFO, message);
        return true;
    }

    bool Server::createHost()
    {
        _server = enet_host_create(&_address, _config.max_clients, 0,
            _config.max_incoming_bandwidth, _config.max_outgoing_bandwidth);
        if (_server == nullptr) {
            Logger::log(LogLevel::CRITICAL,
                "An error occurred while creating the server.");
            return false;
        }
        const std::string message =
            "Server created with max clients: " + std::to_string(_config.max_clients)
            + ", max incoming bandwidth: "
            + std::to_string(_config.max_incoming_bandwidth)
            + ", max outgoing bandwidth: "
            + std::to_string(_config.max_outgoing_bandwidth);
        Logger::log(LogLevel::DEBUG, message);
        return true;
    }

    void Server::listen()
    {
        ENetEvent event;
        while (not should_exit && _running
            && enet_host_service(_server, &event, _config.client_timeout) >= 0) {
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
    }

    bool Server::handleNewClient(ENetEvent &event)
    {
        std::string message = "New client connected from ";

        char ip[16] = {0};
        enet_address_get_host_ip(&event.peer->address, ip, 16);

        message += ip;
        message += ":";
        message += std::to_string(event.peer->address.port);
        Logger::log(LogLevel::INFO, message);
        return true;
    }

    void Server::handleClientDisconnect(ENetEvent &event)
    {
        std::string message = "Client disconnected from ";

        char ip[16] = {0};
        enet_address_get_host_ip(&event.peer->address, ip, 16);

        message += ip;
        message += ":";
        message += std::to_string(event.peer->address.port);
        Logger::log(LogLevel::INFO, message);
        event.peer->data = nullptr;
    }

    void Server::handleClientPacket(ENetEvent &event)
    {
        std::string message = "Packet received from ";
        message += std::to_string(event.peer->address.host);
        message += ":";
        message += std::to_string(event.peer->address.port);
        Logger::log(LogLevel::INFO, message);
    }

    void Server::handleNone([[maybe_unused]] ENetEvent &event) const
    {
        //Logger::log(LogLevel::DEBUG, "No event occurred");
    }

    Server::~Server()
    {
        enet_host_destroy(_server);
        enet_deinitialize();
        Logger::log(LogLevel::INFO, ENET_DEINIT_SUCCESS);
        Logger::log(LogLevel::INFO, "Server stopped");
    }
}
