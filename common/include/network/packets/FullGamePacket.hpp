// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "Tetris.hpp"
#include "network/APacket.hpp"
#include <cstdint>

namespace tetriq {
    class FullGamePacket : public APacket {
        public:
            FullGamePacket();
            FullGamePacket(uint64_t player_id, const Tetris &game, uint64_t applied_actions);

            PacketId getId() const override;

            /**
             * @returns which player's board this packet updates.
             */
            uint64_t getPlayerId() const;
            const Tetris &getGame() const;
            uint64_t getAppliedActions() const;

            NetworkOStream &operator>>(NetworkOStream &ns) const override;
            NetworkIStream &operator<<(NetworkIStream &ns) override;
            size_t getNetworkSize() const override;
        private:
            uint64_t _player_id;
	    Tetris _game{0, 0};
            uint64_t _applied_actions;
    };
}
