// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "Tetris.hpp"
#include "Block.hpp"
#include "GameAction.hpp"
#include "Tetromino.hpp"
#include <cstddef>
#include <cstdint>
#include <iostream>

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

void tetriq::Tetris::handleGameAction(tetriq::GameAction action)
{
    switch (action) {
    case GameAction::MOVE_LEFT:
        return (void) moveCurrentPiece(-1, 0);
    case GameAction::MOVE_RIGHT:
        return (void) moveCurrentPiece(1, 0);
    case GameAction::MOVE_DOWN:
        return (void) moveCurrentPiece(0, 1);
    case GameAction::DROP:
        return (void) dropCurrentPiece();
    case GameAction::ROTATE:
        return (void) rotateCurrentPiece();
    }
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

    for (uint64_t y = 1; y < _height - 1; ++y) {
        if (isLineFull(y)) {
            for (uint64_t x = 1; x < _width - 1; ++x) {
                if (_blocks[y][x] != BlockType::INDESTRUCTIBLE)
                    _blocks[y][x] = BlockType::EMPTY;
            }
            for (uint64_t i = y; i > 1; --i) {
                for (uint64_t x = 1; x < _width - 1; ++x) {
                    moveBlock({x, i}, {x, i + 1});
                }
            }
        }
    }
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
    size_t size = sizeof(uint64_t) * 5 + sizeof(uint8_t);
    for (const auto &tetro : _nextPieces) {
        size += tetro.getNetworkSize();
    }
    for (const auto &type : _blocks) {
        size += sizeof(uint64_t);
        size += sizeof(BlockType) * type.size();
    }
    return size;
}

bool tetriq::Tetris::isLineFull(uint64_t y) const
{
    for (uint64_t i = 0; i < _width; i++) {
        if (_blocks[y][i] == BlockType::EMPTY)
            return false;
    }
    return true;
}

bool tetriq::Tetris::moveBlock(Position oldPos, Position newPos)
{
    if (_blocks[newPos.y][newPos.x] != BlockType::EMPTY)
        return false;
    _blocks[newPos.y][newPos.x] = _blocks[oldPos.y][oldPos.x];
    _blocks[oldPos.y][oldPos.x] = BlockType::EMPTY;
    return true;
}
