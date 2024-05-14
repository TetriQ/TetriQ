// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "SFMLDisplay.hpp"
#include "Block.hpp"
#include "Tetris.hpp"
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

bool tetriq::SFMLDisplay::loadGame(const Tetris &game)
{
    uint64_t width = (game.getWidth() + SIDEBAR_SIZE) * BLOCK_SIZE;
    uint64_t height = game.getHeight() * BLOCK_SIZE;
    sf::View new_view(sf::FloatRect(0, 0, width, height));

    _window.setSize(sf::Vector2u(width, height));
    _window.setView(new_view);
    return true;
}

bool tetriq::SFMLDisplay::draw(const Tetris &game)
{
    BlockType blockType;
    sf::Vector2u pos;

    _window.clear(sf::Color::Black);
    for (uint64_t x = 0; x < game.getWidth(); x++) {
        for (uint64_t y = 0; y < game.getHeight(); y++) {
            blockType = game.getBlockAt(x, y);
            pos = sf::Vector2u(x * BLOCK_SIZE, y * BLOCK_SIZE);
            drawBlock(pos, blockType);
        }
    }
    drawCurrentTetromino(game);
    drawNextTetromino(game);
    _window.display();
    return true;
}

bool tetriq::SFMLDisplay::handleEvents(Tetris &game)
{
    while (_window.pollEvent(_event)) {
        if (_event.type == sf::Event::Closed)
            goto exit_window;
        if (_event.type == sf::Event::KeyPressed) {
            switch (_event.key.code) {
                case sf::Keyboard::Escape:
                    goto exit_window;
                case sf::Keyboard::Left:
                    (void) game.moveCurrentPiece(-1, 0);
                    continue;
                case sf::Keyboard::Right:
                    (void) game.moveCurrentPiece(1, 0);
                    continue;
                case sf::Keyboard::Up:
                    (void) game.rotateCurrentPiece();
                    continue;
                case sf::Keyboard::Down:
                    (void) game.moveCurrentPiece(0, 1);
                    continue;
                case sf::Keyboard::Space:
                    (void) game.dropCurrentPiece();
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

void tetriq::SFMLDisplay::drawBlock(sf::Vector2u pos, BlockType block)
{
    sf::RectangleShape rec{sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE)};

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

void tetriq::SFMLDisplay::drawTetromino(const Tetromino &tetromino, Position position)
{
    const TetroRotation &shape = tetromino.getTetroRotation();

    for (int i = 0; i < 4; i++) {
        std::tuple<char, char> local_pos = shape.at(i);
        int x = position.x + std::get<0>(local_pos);
        int y = position.y + std::get<1>(local_pos);
        drawBlock(sf::Vector2u(x * BLOCK_SIZE, y * BLOCK_SIZE), tetromino.getType());
    }
}

void tetriq::SFMLDisplay::drawCurrentTetromino(const Tetris &game)
{
    drawTetromino(game.getCurrentPiece(), game.getCurrentPiece().getPosition());
}

void tetriq::SFMLDisplay::drawNextTetromino(const Tetris &game)
{
    Position pos = {(int) (game.getWidth() + SIDEBAR_SIZE / 2) - 2, 1};

    drawTetromino(game.getNextPiece(), pos);
}
