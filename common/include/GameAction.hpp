// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <cstdint>

namespace tetriq {
    enum class GameAction : uint64_t {
        MOVE_LEFT,
        MOVE_RIGHT,
        MOVE_DOWN,
        DROP,
        ROTATE,
    };
}
