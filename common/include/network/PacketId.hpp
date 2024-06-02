// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <cstdint>

namespace tetriq {
    /**
     * Packets starting with S_ are sent from server to client and packets
     * starting with C_ are sent from client to server. Others can be sent
     * either way.
     */
    enum class PacketId : uint64_t {
        TEST,
        S_INIT_GAME,
        S_TICK_GAME,
        S_FULL_GAME,
        C_GAME_ACTION,
        C_FULL_GAME_REQUEST,
        C_POWER_UP,
    };
}
