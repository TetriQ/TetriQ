// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "network/APacket.hpp"
#include <cstdint>

namespace tetriq {
    /**
     * Incremental update of the game state (delta encoding).
     */
    class TickGamePacket : public APacket {
        public:
            TickGamePacket();
            TickGamePacket(uint64_t applied_actions);

            PacketId getId() const override;

            uint64_t getAppliedActions() const;

            NetworkOStream &operator>>(NetworkOStream &ns) const override;
            NetworkIStream &operator<<(NetworkIStream &ns) override;
            size_t getNetworkSize() const override;
        private:
            /**
             * Number of actions that have been applied.
             */
            uint64_t _applied_actions;
    };
}
