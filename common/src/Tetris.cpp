// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "Tetris.hpp"
#include "Block.hpp"
#include "Tetromino.hpp"

tetriq::Tetris::Tetris(size_t width, size_t height)
    : _grace_ticks(0)
    , _game_over(false)
    , _width(width)
    , _height(height)
{
    for (int i = 0; i < 3; i++) {
        _nextPieces.emplace_back();
    }

    _blocks.resize(_height);
    for (size_t i = 0; i < _height; i++)
        _blocks[i].resize(_width);

    for (size_t i = 0; i < _height; i++) {
        for (size_t j = 0; j < _width; j++) {
            if (j == 0 || j == _width - 1 || i == _height - 1 || i == 0)
                _blocks[i][j] = std::make_unique<IndestructibleBlock>(*this);
            else
                _blocks[i][j] = std::make_unique<StandardBlock>(*this,
                    EMPTY);
        }
    }
}

tetriq::Tetris::~Tetris()
{}

uint64_t tetriq::Tetris::getWidth() const
{
    return _width;
}

uint64_t tetriq::Tetris::getHeight() const
{
    return _height;
}

const std::unique_ptr<tetriq::Block> &tetriq::Tetris::getBlockAt(uint64_t x,
    uint64_t y) const
{
    return _blocks[y][x];
}

const tetriq::Tetromino &tetriq::Tetris::getCurrentPiece() const
{
    return _nextPieces.front();
}

tetriq::Tetromino &tetriq::Tetris::getCurrentPiece()
{
    return _nextPieces.front();
}

const tetriq::Tetromino &tetriq::Tetris::getNextPiece() const
{
    return _nextPieces.at(1);
}

bool tetriq::Tetris::moveCurrentPiece(int xOffset, int yOffset)
{
    return getCurrentPiece().move(xOffset, yOffset, *this);
}

bool tetriq::Tetris::rotateCurrentPiece()
{
    return getCurrentPiece().rotate(*this);
}

void tetriq::Tetris::dropCurrentPiece()
{
    getCurrentPiece().drop(*this);
}

void tetriq::Tetris::tick()
{
    if (_game_over)
        return;
    if (_grace_ticks != 0) {
        _grace_ticks--;
        return;
    }

    if (!moveCurrentPiece(0, 1))
        placeTetromino();
}

void tetriq::Tetris::addGraceTicks(uint64_t n)
{
    // Prevents resetting grace ticks to stall the game
    if (_grace_ticks != 0)
        return;
    _grace_ticks = n;
}

bool tetriq::Tetris::isOver() const
{
    return _game_over;
}

// Place the current piece on the board and generate a new one
void tetriq::Tetris::placeTetromino()
{
    Tetromino &currentPiece = _nextPieces[0];
    const Rotation &shape = currentPiece.getBlockRotation();

    for (int i = 0; i < 4; i++) {
        auto pos = shape.at(i);
        int x = currentPiece.getPosition().x + std::get<0>(pos);
        int y = currentPiece.getPosition().y + std::get<1>(pos);

        _blocks[y][x] = std::make_unique<StandardBlock>(*this, currentPiece.getType());
    }
    _nextPieces.erase(_nextPieces.begin());
    _nextPieces.emplace_back();
    if (_nextPieces.back().collides(*this))
        _game_over = true;
}
