// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "Server.hpp"
#include "Channel.hpp"
#include "Logger.hpp"
#include "Messages.hpp"
#include "ServerConfig.hpp"
#include "network/PacketHandler.hpp"

#include <cstdint>
#include <tuple>
#include <utility>

namespace tetriq {
    Server::Server(std::string ip, std::string port)
        : _ip(std::move(ip))
        , _port(std::move(port))
        , _address()
        , _server(nullptr)
        , _channels(1)
    {
        if (init() == false)
            throw ServerInitException();
    }

    Player &Server::getPlayerById(uint64_t id)
    {
        return _players.at(id);
    }

    const ServerConfig &Server::getConfig() const
    {
        return _config;
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
            LogLevel::ERROR << "Failed to set host address: " << _ip << std::endl;
            return false;
        }
        try {
            _address.port = std::stoi(_port);
        } catch (const std::invalid_argument &e) {
            LogLevel::ERROR << "Failed to set host port: " << _port << std::endl;
            return false;
        }
        LogLevel::INFO << "Host set to " << _ip << ":" << _port << std::endl;
        return true;
    }

    bool Server::createHost()
    {
        _server = enet_host_create(&_address,
            _config.max_clients,
            0,
            _config.max_incoming_bandwidth,
            _config.max_outgoing_bandwidth);
        if (_server == nullptr) {
            Logger::log(LogLevel::CRITICAL, "An error occurred while creating the enet host.");
            return false;
        }
        LogLevel::DEBUG << "Server created with max clients: " << _config.max_clients
                        << ", max incoming bandwidth: " << _config.max_incoming_bandwidth
                        << ", max outgoing bandwidth: " << _config.max_outgoing_bandwidth
                        << std::endl;
        return true;
    }

    void Server::listen()
    {
        while (not should_exit && _running) {
            // TODO : global TPS limit
            if (!handleENetEvents())
                break;
            for (Channel &channel : _channels) {
                channel.tick(*this);
            }
        }
    }

    bool Server::handleENetEvents()
    {
        ENetEvent event;
        while (enet_host_service(_server, &event, 0) > 0) {
            switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                    handleNewClient(event);
                    break;
                case ENET_EVENT_TYPE_DISCONNECT:
                    handleClientDisconnect(event);
                    break;
                case ENET_EVENT_TYPE_RECEIVE:
                    handleClientPacket(event);
                    break;
                case ENET_EVENT_TYPE_NONE:
                    handleNone(event);
                    break;
            }
        }
        return true;
    }

    bool Server::handleNewClient(ENetEvent &event)
    {
        event.peer->data = new uint64_t(_network_id_counter);
        _players.emplace(std::piecewise_construct,
            std::forward_as_tuple(_network_id_counter),
            std::forward_as_tuple(_network_id_counter, event.peer, &_channels.front()));
        // TODO : choose a channel intelligently and dont start game instantly
        _network_id_counter++;
        return true;
    }

    void Server::handleClientDisconnect(ENetEvent &event)
    {
        uint64_t network_id = *(uint64_t *) event.peer->data;
        _players.erase(network_id);
        delete (uint64_t *) event.peer->data;
        event.peer->data = nullptr;
    }

    void Server::handleClientPacket(ENetEvent &event)
    {
        uint64_t network_id = *(uint64_t *) event.peer->data;
        PacketHandler::decodePacket(event, {&_players.at(network_id)});
    }

    void Server::handleNone([[maybe_unused]] ENetEvent &event) const
    {
        // Logger::log(LogLevel::DEBUG, "No event occurred");
    }

    Server::~Server()
    {
        enet_host_destroy(_server);
        enet_deinitialize();
        Logger::log(LogLevel::INFO, ENET_DEINIT_SUCCESS);
        Logger::log(LogLevel::INFO, "Server stopped");
    }
}
