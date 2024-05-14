// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "network/NetworkStream.hpp"
#include <cstdint>

namespace tetriq {
    class Position {
        public:
            uint64_t x;
            uint64_t y;
    };
    using pos = Position;

    NetworkOStream &operator>>(const Position &pos, NetworkOStream &os);
    NetworkIStream &operator<<(Position &pos, NetworkIStream &os);
}
