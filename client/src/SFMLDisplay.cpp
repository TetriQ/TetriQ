/*
 * This file is part of TetriQ.
 * Copyright © 2024 Noé Lopez <noelopez@free.fr>
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

#include "SFMLDisplay.hpp"
#include "Tetris.hpp"

#include <SFML/Graphics.hpp>
#include <cstdint>

tetriq::SFMLDisplay::SFMLDisplay()
    : _window(
        sf::VideoMode(
            128,
            128,
            sf::VideoMode::getDesktopMode().bitsPerPixel),
        "TetriQ client",
        sf::Style::Titlebar)
{
}

tetriq::SFMLDisplay::~SFMLDisplay()
{
}

bool tetriq::SFMLDisplay::loadGame(const Tetris &game)
{
    uint64_t width = game.getWidth() * BLOCK_SIZE;
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
            blockType = game.getBlockAt(x, y)->getType();
            pos = sf::Vector2u(x * BLOCK_SIZE, y * BLOCK_SIZE);
            drawBlock(pos, blockType);
        }
    }
    _window.display();
    return true;
}

void tetriq::SFMLDisplay::drawBlock(sf::Vector2u pos, BlockType block)
{
    sf::RectangleShape rec{sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE)};

    rec.setPosition(sf::Vector2f(pos));
    rec.setFillColor(sf::Color::Blue);

    _window.draw(rec);
}
