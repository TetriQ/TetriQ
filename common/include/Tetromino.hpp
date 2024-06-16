// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "Utils.hpp"
#include "Block.hpp"
#include "network/NetworkObject.hpp"
#include <cstdint>

namespace tetriq {
    class ITetris;
    class Tetromino : public NetworkObject {
        public:
            Tetromino();
            explicit Tetromino(BlockType &&type);
            ~Tetromino();

            [[nodiscard]] BlockType getType() const;

            [[nodiscard]] pos getPosition() const;
            [[nodiscard]] int getRotation() const;
            void setRotation(int rotation);
            void setPosition(pos position);
            const TetroRotation &getTetroRotation() const;
            [[nodiscard]] bool move(int x, int y, const ITetris &game);
            [[nodiscard]] bool rotate(const ITetris &game);
            void drop(ITetris &game);
            bool collides(const ITetris &game) const;

            NetworkOStream &operator>>(NetworkOStream &os) const override;
            NetworkIStream &operator<<(NetworkIStream &os) override;
            size_t getNetworkSize() const override;
        private:
            pos _position;
            BlockType _type;
            uint64_t _rotation = 0;
    };
}
