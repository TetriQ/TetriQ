// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "GameAction.hpp"
#include "network/APacket.hpp"
#include "network/PacketId.hpp"
namespace tetriq {    
    class GameActionPacket : public APacket {
        public:
            GameActionPacket();
            GameActionPacket(GameAction action);

            PacketId getId() const override;
            GameAction getAction() const;

            NetworkOStream &operator>>(NetworkOStream &ns) const override;
            NetworkIStream &operator<<(NetworkIStream &ns) override;
            size_t getNetworkSize() const override;

        private:
            GameAction _action;
    };
}
