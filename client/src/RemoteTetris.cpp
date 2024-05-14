// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "RemoteTetris.hpp"
#include "Logger.hpp"
#include "network/TestPacket.hpp"
#include "network/packets/TickGamePacket.hpp"
#include <cstddef>

namespace tetriq {
    RemoteTetris::RemoteTetris(size_t width, size_t height)
        : _server_state(width, height)
        , _client_state(width, height)
    {}

    const Tetris &RemoteTetris::getState() const
    {
        return _client_state;
    }

    bool RemoteTetris::handle(TestPacket &)
    {
        LogLevel::DEBUG << "handled test packet" << std::endl;
        return true;
    }

    bool RemoteTetris::handle(TickGamePacket &packet)
    {
        LogLevel::DEBUG << "received server tick" << std::endl;
        _server_state = packet.getGame();
        _client_state = _server_state;
        return true;
    }
}
