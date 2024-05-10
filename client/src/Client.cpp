// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "Client.hpp"

#include <utility>

namespace tetriq {
    Client::Client(std::string ip, std::string port)
    : _username("Unknown"),
      _server_ip(std::move(ip)),
      _server_port(std::move(port)),
      _address(), _client(nullptr), _server(nullptr)
    {
        if (init() == false)
            throw ClientInitException();
        _client = enet_host_create(nullptr, 1, 0,
            _config.max_incoming_bandwidth, _config.max_outgoing_bandwidth);
        if (_client == nullptr or not setServer())
            throw ClientInitException();
        if (not connectToServer())
            throw ClientConnectionException();

        //TODO: Move it
        ENetEvent _event;
        while (enet_host_service(_client, &_event, _config.server_timeout) >= 0) {
            if (_event.type == ENET_EVENT_TYPE_RECEIVE) {
                log(LogLevel::INFO, "A packet was received");
                enet_packet_destroy(_event.packet);
            }
            if (_event.type == ENET_EVENT_TYPE_DISCONNECT) {
                log(LogLevel::INFO, "Disconnected from the server");
                break;
            }
            break;
        }
        sf::Clock clock;
        if (!_display.loadGame(_tetris))
            return;
        while (true) {
            if (!_display.draw(_tetris))
                break;
            if (!_display.handleEvents(_tetris))
                break;
            if (clock.getElapsedTime() > sf::seconds(1.0 / 5.0)) {
                clock.restart();
                _tetris.tick();
            }
        }
    }

    Client::~Client()
    {
        enet_peer_disconnect_now(_server, 0);
        enet_host_destroy(_client);
        enet_deinitialize();
        log(LogLevel::INFO, ENET_DEINIT_SUCCESS);
        log(LogLevel::INFO, "Client stopped");
    }

    bool Client::init() const
    {
        log(LogLevel::INFO, "Client started");
        if (enet_initialize() != 0) {
            log(LogLevel::CRITICAL, ENET_INIT_ERROR);
            return false;
        }
        log(LogLevel::INFO, ENET_INIT_SUCCESS);
        return true;
    }

    void Client::log(const LogLevel level, const std::string &message) const
    {
        if (_logging)
            _logger.log(level, message);
    }

    bool Client::connectToServer()
    {
        ENetEvent _event;
        _server = enet_host_connect(_client, &_address, 0, 0);
        if (_server == nullptr) {
            log(LogLevel::CRITICAL, "An error occurred while connecting to the server : "
                + _server_ip + ":" + _server_port);
            return false;
        }
        if (enet_host_service(_client, &_event, _config.server_timeout) > 0 and
            _event.type == ENET_EVENT_TYPE_CONNECT) {
            log(LogLevel::INFO, "Connected to the server : "
                + _server_ip + ":" + _server_port);
        } else {
            enet_peer_reset(_server);
            log(LogLevel::CRITICAL, "Failed to connect to the server : "
                + _server_ip + ":" + _server_port);
            return false;
        }
        return true;
    }

    bool Client::setServer()
    {
        if (enet_address_set_host(&_address, _server_ip.c_str()) != 0) {
            const std::string message = "Failed to set server address: " + _server_ip;
            log(LogLevel::ERROR, message);
            return false;
        }
        try {
            _address.port = std::stoi(_server_port);
        } catch (const std::invalid_argument &e) {
            const std::string message = "Failed to set server port: " + _server_port;
            log(LogLevel::ERROR, message);
            return false;
        }
        const std::string message = "Server set to " + _server_ip + ":" + _server_port;
        log(LogLevel::INFO, message);
        return true;
    }
}
