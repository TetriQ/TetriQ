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
    _default_font.loadFromFile("etc/OpenSans-SemiBold.ttf");
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

    sf::View new_view(sf::FloatRect(0, 0, static_cast<float>(width), static_cast<float>(height)));

    _window.setSize(sf::Vector2u(width, height));
    _window.setView(new_view);
    return true;
}

bool tetriq::SFMLDisplay::draw(
    const Client &client, ITetrisIter otherGamesStart, ITetrisIter otherGamesEnd)
{
    _window.clear(sf::Color::Black);
    if (_show_help) {
        displayHelp();
        return true;
    }
    uint64_t index = 1;
    ITetris &game = client.getGame();
    drawGame(game, {0, 0}, BLOCK_SIZE * 2, client.targetId == 0);
    drawCurrentTetromino(game);
    drawNextTetromino(game);
    drawPrediction(game);
    drawPowerUps(game);
    uint64_t x = (game.getWidth() + SIDEBAR_SIZE) * BLOCK_SIZE * 2;
    while (otherGamesStart != otherGamesEnd) {
        drawGame(**otherGamesStart, {x, 0}, BLOCK_SIZE, client.targetId == index);
        x += (*otherGamesStart)->getWidth() * BLOCK_SIZE;
        ++otherGamesStart;
        index++;
    }
    _window.display();
    return true;
}

bool tetriq::SFMLDisplay::handleEvents(Client &client)
{
    static bool is_shift_pressed = false;
    static std::string target;
    ITetris &game = client.getGame();
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
                case sf::Keyboard::LShift:
                    is_shift_pressed = true;
                    target.clear();
                    client.targetId = 0;
                    continue;
                case sf::Keyboard::Q:
                    client.sendPowerUp();
                    continue;
                case sf::Keyboard::H:
                    _show_help = !_show_help;
                    continue;
                case sf::Keyboard::Z:
                    client.targetId--;
                    continue;
                case sf::Keyboard::X:
                    client.targetId++;
                    continue;
                default:;
            }
            if (is_shift_pressed && _event.key.code >= sf::Keyboard::Num0
                && _event.key.code <= sf::Keyboard::Num9) {
                if (target.size() >= 3)
                    target.clear();
                target += std::to_string(_event.key.code - sf::Keyboard::Num0);
                client.targetId = std::stoul(target);
            }
        }
        if (_event.type == sf::Event::KeyReleased) {
            if (_event.key.code == sf::Keyboard::LShift) {
                is_shift_pressed = false;
            }
        }
    }
    return true;
exit_window:
    _window.close();
    return false;
}

void tetriq::SFMLDisplay::drawGame(
    const ITetris &game, Position position, uint64_t block_size, bool is_target)
{
    BlockType blockType;
    sf::Vector2u pos;

    for (uint64_t x = 0; x < game.getWidth(); x++) {
        for (uint64_t y = 0; y < game.getHeight(); y++) {
            blockType = game.getBlockAt(x, y);
            pos = sf::Vector2u(position.x + x * block_size, position.y + y * block_size);
            drawBlock(pos, blockType, block_size, is_target);
        }
    }
}

void tetriq::SFMLDisplay::drawBlock(
    sf::Vector2u pos, BlockType block, uint64_t block_size, bool is_target)
{
    sf::RectangleShape rec{
        sf::Vector2f(static_cast<float>(block_size), static_cast<float>(block_size))};

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
            rec.setFillColor(is_target ? sf::Color(158, 114, 114) : sf::Color(59, 59, 59));
            break;
        case BlockType::EMPTY:
            return;
        case BlockType::PU_ADD_LINE:
            rec.setFillColor(sf::Color::Cyan);
            break;
        case BlockType::PU_GRAVITY:
            rec.setFillColor(sf::Color(70, 70, 165));
            break;
        case BlockType::PU_BLOCK_BOMB:
            rec.setFillColor(sf::Color(80, 175, 162));
            break;
        case BlockType::PU_CLEAR_LINE:
            rec.setFillColor(sf::Color(152, 129, 53));
            break;
        case BlockType::PU_NUKE_FIELD:
            rec.setFillColor(sf::Color(142, 184, 202));
            break;
        case BlockType::PU_COLUMN_SHUFFLE:
            rec.setFillColor(sf::Color(83, 137, 36));
            break;
        case BlockType::PU_SWITCH_FIELD:
            rec.setFillColor(sf::Color(158, 80, 111));
            break;
        case BlockType::PU_CLEAR_BLOCK_RANDOM:
            rec.setFillColor(sf::Color(139, 5, 59));
            break;
        case BlockType::PU_CLEAR_SPECIAL_BLOCK:
            rec.setFillColor(sf::Color(5, 90, 139));
            break;
        default:
            rec.setFillColor(sf::Color::White);
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
        unsigned int x = (position.x + std::get<0>(local_pos)) * block_size;
        unsigned int y = (position.y + std::get<1>(local_pos)) * block_size;
        drawBlock(sf::Vector2u(x, y), tetromino.getType(), block_size, false);
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

bool tetriq::SFMLDisplay::skipOverlap(
    Position pos, const TetroRotation &shape, unsigned int tempx, unsigned int &tempy)
{
    bool is_current = false;
    for (int j = 0; j < 4; j++) {
        std::tuple<char, char> local_pos = shape.at(j);
        if (tempx == pos.x + std::get<0>(local_pos) && tempy == pos.y + std::get<1>(local_pos)) {
            is_current = true;
            break;
        }
    }
    if (is_current) {
        tempy++;
        return true;
    }
    return false;
}

void tetriq::SFMLDisplay::drawPrediction(const ITetris &game)
{
    const Tetromino &current = game.getCurrentPiece();
    Position pos = current.getPosition();
    const TetroRotation &shape = current.getTetroRotation();

    for (int i = 0; i < 4; i++) {
        std::tuple<char, char> local_pos = shape.at(i);
        unsigned int tempx = pos.x + std::get<0>(local_pos);
        unsigned int tempy = pos.y + std::get<1>(local_pos);
        unsigned int offset = 0;
        while (tempy < game.getHeight() && game.getBlockAt(tempx, tempy) == BlockType::EMPTY) {
            if (offset < 4 && skipOverlap(pos, shape, tempx, tempy))
                continue;
            constexpr float radius = 1;
            sf::RectangleShape point(sf::Vector2f(radius, radius));
            point.setFillColor(sf::Color::White);
            point.setPosition(sf::Vector2f(
                static_cast<float>(tempx * BLOCK_SIZE * 2) + static_cast<float>(BLOCK_SIZE),
                static_cast<float>(tempy * BLOCK_SIZE * 2) + static_cast<float>(BLOCK_SIZE)));
            tempy++;
            offset++;
            _window.draw(point);
        }
    }
}

void tetriq::SFMLDisplay::drawPowerUps(const ITetris &game)
{
    const auto it = game.getPowerUps().begin();
    const auto end = game.getPowerUps().end();

    Position pos = {game.getWidth() + 1, 4};
    for (auto i = it; i != end; ++i) {
        drawBlock(sf::Vector2u(pos.x * BLOCK_SIZE * 2, pos.y * BLOCK_SIZE * 2),
            *i,
            BLOCK_SIZE * 2,
            false);
        pos.y += 2;
    }
}

void tetriq::SFMLDisplay::displayHelp()
{
    uint64_t x = 0;
    uint64_t y = 0;
    for (uint64_t i = 0; i < static_cast<uint64_t>(BlockType::BLOCKTYPE_COUNT); i++) {
        auto block = static_cast<BlockType>(i);
        drawBlock(sf::Vector2u(x, y), block, BLOCK_SIZE, false);
        sf::Text text;
        text.setFont(_default_font);
        text.setString(std::to_string(i) + " " + blockTypeToString(block));
        text.setCharacterSize(12);
        text.setFillColor(sf::Color::White);
        text.setPosition(x + BLOCK_SIZE, y);
        _window.draw(text);
        y += BLOCK_SIZE;
    }
    _window.display();
}
