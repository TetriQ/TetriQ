// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "Tetromino.hpp"
#include "Block.hpp"
#include "Utils.hpp"
#include "Tetris.hpp"
#include <cstdint>
#include <tuple>

// create a tetromino at x=4 y=0 && with a random shape
tetriq::Tetromino::Tetromino()
    : _position({4, 1})
    , _type(static_cast<BlockType>(rand() % 7 + 1))
{}

tetriq::Tetromino::Tetromino(BlockType &&type)
    : _position({4, 1})
    , _type(type)
{}

tetriq::Tetromino::~Tetromino() = default;

tetriq::BlockType tetriq::Tetromino::getType() const
{
    return _type;
}

// return the position of the tetromino
tetriq::pos tetriq::Tetromino::getPosition() const
{
    return _position;
}

int tetriq::Tetromino::getRotation() const
{
    return _rotation;
}

void tetriq::Tetromino::setRotation(int rotation)
{
    _rotation = rotation;
}

// set the position of the tetromino
void tetriq::Tetromino::setPosition(pos position)
{
    _position = position;
}

const tetriq::TetroRotation &tetriq::Tetromino::getTetroRotation() const
{
    return BLOCK_ROTATIONS.at(getType()).at(getRotation());
}

bool tetriq::Tetromino::move(int xOffset, int yOffset, const ITetris &game)
{
    Tetromino next = *this;
    next._position.x += xOffset;
    next._position.y += yOffset;
    if (next.collides(game))
        return false;

    _position = next._position;
    return true;
}

bool tetriq::Tetromino::rotate(const ITetris &game)
{
    Tetromino next = *this;
    next._rotation = (_rotation + 1) % BLOCK_ROTATIONS.at(_type).size();
    if (next.collides(game))
        return false;

    _rotation = next._rotation;
    return true;
}

void tetriq::Tetromino::drop(ITetris &game)
{
    while (move(0, 1, game)) {}
    // game.addGraceTicks(1);
}

bool tetriq::Tetromino::collides(const ITetris &game) const
{
    const TetroRotation &shape = getTetroRotation();
    for (int i = 0; i < 4; i++) {
        const std::tuple<char, char> &local_pos = shape.at(i);
        int x = _position.x + std::get<0>(local_pos);
        int y = _position.y + std::get<1>(local_pos);
        if (x < 0 || x >= static_cast<int>(game.getWidth())
            || y >= static_cast<int>(game.getHeight()))
            return true;
        if (game.getBlockAt(x, y) != BlockType::EMPTY)
            return true;
    }
    return false;
}

tetriq::NetworkOStream &tetriq::Tetromino::operator>>(tetriq::NetworkOStream &os) const
{
    _position >> os;
    _type >> os;
    _rotation >> os;
    return os;
}

tetriq::NetworkIStream &tetriq::Tetromino::operator<<(tetriq::NetworkIStream &os)
{
    _position << os;
    _type << os;
    _rotation << os;
    return os;
}

size_t tetriq::Tetromino::getNetworkSize() const
{
    return sizeof(uint64_t) * 4;
}
