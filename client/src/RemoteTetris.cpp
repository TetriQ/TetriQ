// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "RemoteTetris.hpp"
#include "GameAction.hpp"
#include "Logger.hpp"
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
    {}

    bool RemoteTetris::handleGameAction(GameAction action)
    {
        GameActionPacket packet{action};
        packet.send(_peer);
        _client_state.handleGameAction(action);
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
        return true;
    }

    bool RemoteTetris::handle(FullGamePacket &packet)
    {
        if (packet.getPlayerId() != _player_id)
            return false;
        _client_state = packet.getGame();
        _server_state = _client_state;
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
