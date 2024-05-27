// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "RemoteTetris.hpp"
#include "GameAction.hpp"
#include "Logger.hpp"
#include "network/packets/FullGameRequestPacket.hpp"
#include "network/packets/TestPacket.hpp"
#include "network/packets/GameActionPacket.hpp"
#include "network/packets/FullGamePacket.hpp"
#include "network/packets/TickGamePacket.hpp"
#include <cassert>
#include <cstddef>
#include <cstdint>

namespace tetriq {
    RemoteTetris::RemoteTetris(size_t width, size_t height, ENetPeer *peer, uint64_t player_id)
        : _peer(peer)
        , _player_id(player_id)
        , _server_state(width, height)
        , _client_state(width, height)
        , _resyncing(true) // at the start of game we wait for the server's FullGamePacket
    {}

    bool RemoteTetris::handleGameAction(GameAction action)
    {
        if (_resyncing)
            return true;
        GameActionPacket packet{action};
        packet.send(_peer);
        if (action == GameAction::DROP)
            _resyncing = true;
        if (_client_state.handleGameAction(action)) {
            LogLevel::DEBUG
                << "adding action"
                << static_cast<uint64_t>(action)
                << std::endl;
            _actions.push(action);
        }
        return true;
    }

    bool RemoteTetris::handle(TestPacket &)
    {
        LogLevel::DEBUG << "handled test packet" << std::endl;
        return true;
    }

    bool RemoteTetris::handle(TickGamePacket &packet)
    {
        if (packet.getPlayerId() != _player_id)
            return false;
        for (GameAction action : packet.getActions()) {
            if (_actions.empty()) {
                LogLevel::DEBUG << "received extraneous server action" << std::endl;
                break;
            }
            LogLevel::INFO << static_cast<uint64_t>(action)
                           << "="
                           << static_cast<uint64_t>(_actions.front())
                           << std::endl;
            if (_actions.front() == action && _server_state.handleGameAction(action)) {
                _actions.pop();
            } else {
                LogLevel::ERROR << "server desynchronisation" << std::endl;
                FullGameRequestPacket{}.send(_peer);
                _resyncing = true;
                break;
            }
        }
        _client_state.tick();
        _server_state.tick();
        return true;
    }

    bool RemoteTetris::handle(FullGamePacket &packet)
    {
        if (packet.getPlayerId() != _player_id)
            return false;
        _server_state = packet.getGame();
        if (_resyncing) {
            _client_state = _server_state;
            while (!_actions.empty())
                _actions.pop();
            _resyncing = false;
        }
        return true;
    }

    uint64_t RemoteTetris::getWidth() const
    {
        return _client_state.getWidth();
    }

    uint64_t RemoteTetris::getHeight() const
    {
        return _client_state.getHeight();
    }

    BlockType RemoteTetris::getBlockAt(uint64_t x, uint64_t y) const
    {
        return _client_state.getBlockAt(x, y);
    }

    const Tetromino &RemoteTetris::getCurrentPiece() const
    {
        return _client_state.getCurrentPiece();
    }

    const Tetromino &RemoteTetris::getNextPiece() const
    {
        return _client_state.getNextPiece();
    }
}
