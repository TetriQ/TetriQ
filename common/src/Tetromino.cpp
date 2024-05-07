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

#include <utility>

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

// return the type of the tetromino
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
