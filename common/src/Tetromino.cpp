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

tetriq::Tetromino::Tetromino() : _position({0, 0})
{}

tetriq::Tetromino::Tetromino(const pos &position) : _position(position)
{}

tetriq::Tetromino::~Tetromino() = default;

tetriq::pos tetriq::Tetromino::getPosition() const
{
    return _position;
}

void tetriq::Tetromino::setPosition(pos position)
{
    _position = position;
}