// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <cstdint>

namespace tetriq {
    class Player {
        public:
            Player(uint64_t network_id);
            ~Player();

            uint64_t getNetworkId() const;
        private:
            const uint64_t _network_id;
    };
}
