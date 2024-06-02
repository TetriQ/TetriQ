// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "Tetris.hpp"
#include "Block.hpp"
#include "GameAction.hpp"
#include "Logger.hpp"
#include "Tetromino.hpp"
#include <cstddef>
#include <cstdint>

void tetriq::Tetris::createBorders(
    std::vector<std::vector<BlockType>> &_board, uint64_t width, uint64_t height)
{
    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            if (j == 0 || j == width - 1 || i == height - 1 || i == 0)
                _board[i][j] = BlockType::INDESTRUCTIBLE;
            else
                _board[i][j] = BlockType::EMPTY;
        }
    }
}

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

    createBorders(_blocks, _width, _height);
}

tetriq::Tetris::~Tetris() = default;

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

bool tetriq::Tetris::handleGameAction(tetriq::GameAction action)
{
    switch (action) {
        case GameAction::MOVE_LEFT:
            return moveCurrentPiece(-1, 0);
        case GameAction::MOVE_RIGHT:
            return moveCurrentPiece(1, 0);
        case GameAction::MOVE_DOWN:
            return moveCurrentPiece(0, 1);
        case GameAction::DROP:
            return dropCurrentPiece(), true;
        case GameAction::ROTATE:
            return rotateCurrentPiece();
        default:
            LogLevel::WARNING << "Unknown game action" << std::endl;
            return false;
    }
}

tetriq::BlockType tetriq::Tetris::consumePowerUp()
{
    if (_powerUps.empty())
        return BlockType::EMPTY;
    const BlockType powerUp = _powerUps.front();
    _powerUps.pop_front();
    return powerUp;
}

void tetriq::Tetris::doPuAddLine()
{
    __attribute_maybe_unused__ bool has_moved = moveCurrentPiece(0, -1);
    moveBlocksUp(_height - 2);
    uint64_t random = rand() % (_width - 2) + 1;
    for (uint64_t x = 1; x < _width - 1; ++x) {
        _blocks[_height - 2][x] = BlockType::RED;
    }
    _blocks[_height - 2][random] = BlockType::EMPTY;
}

void tetriq::Tetris::doPuClearLine()
{
    clearLine(_height - 2);
    moveBlocksDown(_height - 2);
}

void tetriq::Tetris::doPuClearSpecialBlock()
{
    for (uint64_t y = 1; y < _height - 1; ++y) {
        for (uint64_t x = 1; x < _width - 1; ++x) {
            if (_blocks[y][x] > BlockType::INDESTRUCTIBLE) {
                _blocks[y][x] = BlockType::EMPTY;
            }
        }
    }
}

void tetriq::Tetris::doPuClearBlockRandom()
{
    auto blocks = getBlocks();
    if (blocks.empty())
        return;
    uint64_t blocks_to_clear = blocks.size() * 0.3;
    for (uint64_t i = 0; i < blocks_to_clear; i++) {
        uint64_t random_block = rand() % blocks.size();
        _blocks[blocks[random_block].y][blocks[random_block].x] = BlockType::EMPTY;
        blocks.erase(blocks.begin() + random_block);
    }
}

void tetriq::Tetris::doPuGravity()
{
    for (uint64_t y = _height - 2; y > 0; --y) {
        for (uint64_t x = 1; x < _width - 1; ++x) {
            if (_blocks[y][x] != BlockType::EMPTY && _blocks[y][x] != BlockType::INDESTRUCTIBLE) {
                while (moveBlock({x, y}, {x, y + 1})) {
                    y = y + 1;
                }
            }
        }
    }
}

void tetriq::Tetris::doPuNukeField()
{
    for (uint64_t y = 1; y < _height - 1; ++y) {
        for (uint64_t x = 1; x < _width - 1; ++x) {
            if (_blocks[y][x] != BlockType::INDESTRUCTIBLE) {
                _blocks[y][x] = BlockType::EMPTY;
            }
        }
    }
    while (not _powerUps.empty())
        _powerUps.pop_front();
}

void tetriq::Tetris::doPuColumnShuffle()
{
    static std::random_device rd;
    static std::mt19937 g(rd());

    std::vector<uint64_t> columns;
    for (uint64_t i = 1; i < _width - 1; i++) {
        columns.push_back(i);
    }
    std::ranges::shuffle(columns, g);
    std::vector new_blocks(_height, std::vector(_width, BlockType::EMPTY));
    createBorders(new_blocks, _width, _height);
    for (uint64_t y = 1; y < _height - 1; y++) {
        for (uint64_t x = 1; x < _width - 1; x++) {
            new_blocks[y][columns[x - 1]] = _blocks[y][x];
        }
    }
    _blocks = new_blocks;
}

void tetriq::Tetris::applyPowerUp(BlockType powerUp)
{
    switch (powerUp) {
        case BlockType::EMPTY:
        case BlockType::RED:
        case BlockType::BLUE:
        case BlockType::DARK_BLUE:
        case BlockType::ORANGE:
        case BlockType::YELLOW:
        case BlockType::GREEN:
        case BlockType::PURPLE:
        case BlockType::INDESTRUCTIBLE:
            break;
        case BlockType::PU_ADD_LINE:
            doPuAddLine();
            break;
        case BlockType::PU_CLEAR_SPECIAL_BLOCK:
            doPuClearSpecialBlock();
            break;
        case BlockType::PU_CLEAR_LINE:
            doPuClearLine();
            break;
        case BlockType::PU_CLEAR_BLOCK_RANDOM:
            doPuClearBlockRandom();
            break;
        case BlockType::PU_GRAVITY:
            doPuGravity();
            break;
        case BlockType::PU_NUKE_FIELD:
            doPuNukeField();
            break;
        case BlockType::PU_BLOCK_BOMB:
            {
                break;
            }
        case BlockType::PU_COLUMN_SHUFFLE:
            doPuColumnShuffle();
            break;
        case BlockType::PU_SWITCH_FIELD:
            break;
        default:
            LogLevel::WARNING << "Unknown power up" << std::endl;
            break;
    }
}

void tetriq::Tetris::clearLine(uint64_t y)
{
    for (uint64_t x = 1; x < _width - 1; ++x) {
        if (_blocks[y][x] != BlockType::INDESTRUCTIBLE) {
            if (_blocks[y][x] > BlockType::INDESTRUCTIBLE) {
                _powerUps.emplace_back(_blocks[y][x]);
            }
            _blocks[y][x] = BlockType::EMPTY;
        }
    }
}

void tetriq::Tetris::moveBlocksDown(uint64_t y)
{
    for (uint64_t i = y; i > 1; --i) {
        for (uint64_t x = 1; x < _width - 1; ++x) {
            moveBlock({x, i}, {x, i + 1});
        }
    }
}

void tetriq::Tetris::moveBlocksUp(uint64_t y)
{
    for (uint64_t i = 1; i < y + 1; ++i) {
        for (uint64_t x = 1; x < _width - 1; ++x) {
            moveBlock({x, i}, {x, i - 1});
        }
    }
}

void tetriq::Tetris::removeLinesFulls(bool &changed, unsigned int &lines_deleted)
{
    for (uint64_t y = 1; y < _height - 1; ++y) {
        if (isLineFull(y)) {
            changed = true;
            lines_deleted++;
            clearLine(y);
            moveBlocksDown(y - 1);
        }
    }
}

uint64_t tetriq::Tetris::getMaxHeight() const
{
    for (uint64_t y = 0; y < _height; y++) {
        for (uint64_t x = 0; x < _width; x++) {
            if (_blocks[y][x] != BlockType::EMPTY && _blocks[y][x] != BlockType::INDESTRUCTIBLE) {
                return y;
            }
        }
    }
    return _height;
}

const std::deque<tetriq::BlockType> &tetriq::Tetris::getPowerUps() const
{
    return _powerUps;
}

bool tetriq::Tetris::tick()
{
    bool changed = false;
    unsigned int lines_deleted = 0;

    _tick++;
    if (_game_over)
        return changed;
    if (_grace_ticks != 0) {
        _grace_ticks--;
        return changed;
    }
    if (!moveCurrentPiece(0, 1)) {
        placeTetromino();
        changed = true;
    }
    removeLinesFulls(changed, lines_deleted);
    uint64_t block_on_board = countBlocks();
    if (block_on_board == 0 || lines_deleted == 0)
        return changed;

    unsigned int max_height = getMaxHeight();
    std::vector<std::tuple<uint64_t, uint64_t>> blocks_in_4_next_lines;
    for (uint64_t y = max_height - 1; y < max_height + 4 && y < _height; y++) {
        for (uint64_t x = 0; x < _width; x++) {
            if (_blocks[y][x] != BlockType::EMPTY && _blocks[y][x] < BlockType::INDESTRUCTIBLE) {
                blocks_in_4_next_lines.emplace_back(x, y);
            }
        }
    }
    for (unsigned int i = 0; i < lines_deleted; i++) {
        uint64_t random_block = rand() % blocks_in_4_next_lines.size();
        uint64_t random_block_x = std::get<0>(blocks_in_4_next_lines[random_block]);
        uint64_t random_block_y = std::get<1>(blocks_in_4_next_lines[random_block]);
        _blocks[random_block_y][random_block_x] = WeightedPowerUp::getRandom();
        changed = true;
        blocks_in_4_next_lines.erase(blocks_in_4_next_lines.begin() + random_block);
        if (blocks_in_4_next_lines.empty())
            break;
    }
    return changed;
}

void tetriq::Tetris::addGraceTicks(uint64_t n)
{
    // Prevents resetting grace ticks to stall the game
    if (_grace_ticks != 0)
        return;
    _grace_ticks = n;
}

uint64_t tetriq::Tetris::getCurrentTick() const
{
    return _tick;
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
        const uint64_t x = currentPiece.getPosition().x + std::get<0>(pos);
        const uint64_t y = currentPiece.getPosition().y + std::get<1>(pos);

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
    _tick >> os;
    _powerUps >> os;
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
    _tick << os;
    _powerUps << os;
    _game_over = game_over;
    return os;
}

size_t tetriq::Tetris::getNetworkSize() const
{
    size_t size = sizeof(uint64_t) * 7 + sizeof(uint8_t);
    size_t size = sizeof(uint64_t) * 6 + sizeof(uint8_t);
    for (const auto &tetro : _nextPieces) {
        size += tetro.getNetworkSize();
    }
    for (const auto &type : _blocks) {
        size += sizeof(uint64_t);
        size += sizeof(BlockType) * type.size();
    }
    size += sizeof(uint64_t) * _powerUps.size();
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

uint64_t tetriq::Tetris::countBlocks() const
{
    uint64_t count = 0;
    for (const auto &row : _blocks) {
        for (const auto &block : row) {
            if (block != BlockType::EMPTY && block < BlockType::INDESTRUCTIBLE) {
                count++;
            }
        }
    }
    return count;
}

std::vector<tetriq::Position> tetriq::Tetris::getBlocks() const
{
    std::vector<Position> blocks;
    for (uint64_t y = 0; y < _height; y++) {
        for (uint64_t x = 0; x < _width; x++) {
            if (_blocks[y][x] != BlockType::EMPTY && _blocks[y][x] != BlockType::INDESTRUCTIBLE) {
                blocks.emplace_back(x, y);
            }
        }
    }
    return blocks;
}
