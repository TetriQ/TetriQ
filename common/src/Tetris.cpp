// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "Tetris.hpp"
#include "Block.hpp"
#include "Tetromino.hpp"
#include <cstddef>
#include <cstdint>
#include <memory>

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
                _blocks[i][j] = BlockType::INDESTRUCTIBLE;
            else
                _blocks[i][j] = BlockType::EMPTY;
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

tetriq::BlockType tetriq::Tetris::getBlockAt(uint64_t x, uint64_t y) const
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

const std::vector<tetriq::Tetromino> &tetriq::Tetris::getNextPieces() const
{
    return _nextPieces;
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
    const TetroRotation &shape = currentPiece.getTetroRotation();

    for (int i = 0; i < 4; i++) {
        auto pos = shape.at(i);
        int x = currentPiece.getPosition().x + std::get<0>(pos);
        int y = currentPiece.getPosition().y + std::get<1>(pos);

        _blocks[y][x] = currentPiece.getType();
    }
    _nextPieces.erase(_nextPieces.begin());
    _nextPieces.emplace_back();
    if (_nextPieces.back().collides(*this))
        _game_over = true;
}

tetriq::NetworkOStream &tetriq::Tetris::operator>>(tetriq::NetworkOStream &os) const
{
    _grace_ticks >> os;
    (uint8_t) _game_over >> os;
    _width >> os;
    _height >> os;
    _blocks >> os;
    _nextPieces >> os;
    return os;
}

tetriq::NetworkIStream &tetriq::Tetris::operator<<(tetriq::NetworkIStream &os)
{
    uint8_t game_over;

    _grace_ticks << os;
    game_over << os;
    _width << os;
    _height << os;
    _blocks << os;
    _nextPieces << os;
    _game_over = game_over;
    return os;
}

size_t tetriq::Tetris::getNetworkSize() const
{
    size_t size = sizeof(uint64_t) * 5
        + sizeof(bool)
        + sizeof(Tetromino) * _nextPieces.size()
        + sizeof(uint64_t) * _blocks.size();
    for (const auto &type : _blocks) {
        size += sizeof(BlockType) * type.size();
    }
    return size;
}
