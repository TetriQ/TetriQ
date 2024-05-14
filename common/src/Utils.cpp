// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "Utils.hpp"

namespace tetriq {
    NetworkOStream &operator>>(const Position &pos, NetworkOStream &os)
    {
        pos.x >> os;
        pos.y >> os;
        return os;
    }

    NetworkIStream &operator<<(Position &pos, NetworkIStream &os)
    {
        pos.x << os;
        pos.y << os;
        return os;
    }
}
