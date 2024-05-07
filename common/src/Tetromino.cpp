/*
* This file is part of TetriQ.
* Copyright © 2024 Sébastien Lucas <sebastien.lucas@epitech.eu>
*
* TetriQ is free software: you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* TetriQ is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with TetriQ. If not, see <https://www.gnu.org/licenses/>.
*/

#include "Tetromino.hpp"
#include "Block.hpp"
#include "Utils.hpp"
#include "Tetris.hpp"
#include <tuple>

// create a tetromino at x=4 y=0 && with a random shape
tetriq::Tetromino::Tetromino()
: _position({4, 1}), _type(static_cast<BlockType>(rand() % 7 + 1))
{
}

tetriq::Tetromino::Tetromino(BlockType &&type) : _position({4, 1}),
_type(type)
{
}

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

const tetriq::Rotation &tetriq::Tetromino::getBlockRotation() const
{
    return BLOCK_ROTATIONS.at(getType()).at(getRotation());
}

bool tetriq::Tetromino::move(int xOffset, int yOffset, const Tetris &game)
{
    Tetromino next = *this;
    next._position.x += xOffset;
    next._position.y += yOffset;
    if (next.collides(game))
        return false;

    _position = next._position;
    return true;
}

bool tetriq::Tetromino::rotate(const Tetris &game)
{
    Tetromino next = *this;
    next._rotation = (_rotation + 1) % BLOCK_ROTATIONS.at(_type).size();
    if (next.collides(game))
        return false;

    _rotation = next._rotation;
    return true;
}

void tetriq::Tetromino::drop(Tetris &game)
{
    while (move(0, 1, game));
    game.addGraceTicks(1);
}

bool tetriq::Tetromino::collides(const Tetris &game)
{
    const Rotation &shape = getBlockRotation();
    for (int i = 0; i < 4; i++) {
        const std::tuple<char, char> &local_pos = shape.at(i);
        int x = _position.x + std::get<0>(local_pos);
        int y = _position.y + std::get<1>(local_pos);
        if (x < 0 || x >= static_cast<int> (game.getWidth()) || y >= static_cast<int>(game.getHeight()))
            return true;
        if (game.getBlockAt(x, y)->getType() != EMPTY)
            return true;
    }
    return false;
}
