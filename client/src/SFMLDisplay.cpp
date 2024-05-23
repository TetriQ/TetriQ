// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "SFMLDisplay.hpp"
#include "Block.hpp"
#include "GameAction.hpp"
#include "ITetris.hpp"
#include "Logger.hpp"
#include "Utils.hpp"

#include <SFML/Graphics.hpp>
#include <cstdint>

tetriq::SFMLDisplay::SFMLDisplay()
    : _window(sf::VideoMode(128, 128, sf::VideoMode::getDesktopMode().bitsPerPixel),
          "TetriQ client", sf::Style::Titlebar | sf::Style::Close)
    , _event()
{
    _window.setFramerateLimit(60);
}

tetriq::SFMLDisplay::~SFMLDisplay()
{}

bool tetriq::SFMLDisplay::loadGame(const ITetris &game, uint64_t player_count)
{
    uint64_t board_width = (game.getWidth() + SIDEBAR_SIZE) * BLOCK_SIZE * 2;
    uint64_t board_height = game.getHeight() * BLOCK_SIZE * 2;
    uint64_t other_boards_width = player_count * game.getWidth() * BLOCK_SIZE;
    uint64_t width = board_width + other_boards_width;
    uint64_t height = board_height;

    sf::View new_view(sf::FloatRect(0, 0, width, height));

    _window.setSize(sf::Vector2u(width, height));
    _window.setView(new_view);
    return true;
}

bool tetriq::SFMLDisplay::draw(
    const ITetris &game, ITetrisIter otherGamesStart, ITetrisIter otherGamesEnd)
{
    _window.clear(sf::Color::Black);
    drawGame(game, {0, 0}, BLOCK_SIZE * 2);
    drawCurrentTetromino(game);
    drawNextTetromino(game);
    uint64_t x = (game.getWidth() + SIDEBAR_SIZE) * BLOCK_SIZE * 2;
    while (otherGamesStart != otherGamesEnd) {
        drawGame(**otherGamesStart, {x, 0}, BLOCK_SIZE);
        x += (*otherGamesStart)->getWidth() * BLOCK_SIZE;
        ++otherGamesStart;
    }
    _window.display();
    return true;
}

bool tetriq::SFMLDisplay::handleEvents(ITetris &game)
{
    while (_window.pollEvent(_event)) {
        if (_event.type == sf::Event::Closed)
            goto exit_window;
        if (_event.type == sf::Event::KeyPressed) {
            switch (_event.key.code) {
                case sf::Keyboard::Escape:
                    goto exit_window;
                case sf::Keyboard::Left:
                    game.handleGameAction(GameAction::MOVE_LEFT);
                    continue;
                case sf::Keyboard::Right:
                    game.handleGameAction(GameAction::MOVE_RIGHT);
                    continue;
                case sf::Keyboard::Up:
                    game.handleGameAction(GameAction::ROTATE);
                    continue;
                case sf::Keyboard::Down:
                    game.handleGameAction(GameAction::MOVE_DOWN);
                    continue;
                case sf::Keyboard::Space:
                    game.handleGameAction(GameAction::DROP);
                    continue;
                default:
                    continue;
            }
        }
    }
    return true;
exit_window:
    _window.close();
    return false;
}

void tetriq::SFMLDisplay::drawGame(const ITetris &game, Position position, uint64_t block_size)
{
    BlockType blockType;
    sf::Vector2u pos;

    for (uint64_t x = 0; x < game.getWidth(); x++) {
        for (uint64_t y = 0; y < game.getHeight(); y++) {
            blockType = game.getBlockAt(x, y);
            pos = sf::Vector2u(position.x + x * block_size, position.y + y * block_size);
            drawBlock(pos, blockType, block_size);
        }
    }
}

void tetriq::SFMLDisplay::drawBlock(sf::Vector2u pos, BlockType block, uint64_t block_size)
{
    sf::RectangleShape rec{sf::Vector2f(block_size, block_size)};

    rec.setPosition(sf::Vector2f(pos));
    switch (block) {
        case BlockType::RED:
            rec.setFillColor(sf::Color::Red);
            break;
        case BlockType::BLUE:
            rec.setFillColor(sf::Color::Blue);
            break;
        case BlockType::DARK_BLUE:
            rec.setFillColor(sf::Color(0, 0, 139));
            break;
        case BlockType::ORANGE:
            rec.setFillColor(sf::Color(255, 165, 0));
            break;
        case BlockType::YELLOW:
            rec.setFillColor(sf::Color::Yellow);
            break;
        case BlockType::GREEN:
            rec.setFillColor(sf::Color::Green);
            break;
        case BlockType::PURPLE:
            rec.setFillColor(sf::Color::Magenta);
            break;
        case BlockType::INDESTRUCTIBLE:
            rec.setFillColor(sf::Color(59, 59, 59));
            break;
        case BlockType::EMPTY:
            rec.setFillColor(sf::Color::Black);
            break;
        case BlockType::SPECIAL:
            rec.setFillColor(sf::Color::Cyan);
            break;
    }

    _window.draw(rec);
}

void tetriq::SFMLDisplay::drawTetromino(
    const Tetromino &tetromino, Position position, uint64_t block_size)
{
    const TetroRotation &shape = tetromino.getTetroRotation();

    for (int i = 0; i < 4; i++) {
        std::tuple<char, char> local_pos = shape.at(i);
        int x = (position.x + std::get<0>(local_pos)) * block_size;
        int y = (position.y + std::get<1>(local_pos)) * block_size;
        drawBlock(sf::Vector2u(x, y), tetromino.getType(), block_size);
    }
}

void tetriq::SFMLDisplay::drawCurrentTetromino(const ITetris &game)
{
    drawTetromino(game.getCurrentPiece(), game.getCurrentPiece().getPosition(), BLOCK_SIZE * 2);
}

void tetriq::SFMLDisplay::drawNextTetromino(const ITetris &game)
{
    Position pos = {game.getWidth() + 1, 1};

    drawTetromino(game.getNextPiece(), pos, BLOCK_SIZE * 2);
}
