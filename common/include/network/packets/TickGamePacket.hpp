// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "GameAction.hpp"
#include "network/APacket.hpp"
#include <cstdint>
#include <vector>

namespace tetriq {
    class TickGamePacket : public APacket {
        public:
            TickGamePacket();
            template<class InputIterator>
            TickGamePacket(uint64_t player_id, InputIterator actionsBegin, InputIterator actionsEnd);

            PacketId getId() const override;

            uint64_t getPlayerId() const;
            const std::vector<GameAction> &getActions() const;

            NetworkOStream &operator>>(NetworkOStream &ns) const override;
            NetworkIStream &operator<<(NetworkIStream &ns) override;
            size_t getNetworkSize() const override;
        private:
            uint64_t _player_id;
            std::vector<GameAction> _actions;
    };

    template<class InputIterator>
    TickGamePacket::TickGamePacket(uint64_t player_id, InputIterator actionsBegin, InputIterator actionsEnd)
        : _player_id(player_id)
        , _actions(actionsBegin, actionsEnd)
    {}
}
