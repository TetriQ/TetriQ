// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "NcursesDisplay.hpp"
#include "Block.hpp"
#include "GameAction.hpp"
#include "ITetris.hpp"
#include "Logger.hpp"
#include "Utils.hpp"

tetriq::NcursesDisplay::NcursesDisplay()
{
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    start_color();
    _scr_width = getmaxx(stdscr);
    _scr_height = getmaxy(stdscr);
    _win_height = _scr_height / 10 * 9 >= 3 ? _scr_height / 10 * 9 : 3;
    _win_width = _scr_width;
    _window = newwin(_win_height, _win_width, 10, 10);
    box(_window, 0, 0);
    mvwprintw(_window, 0, 2, "TetriQ");

    init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
}

tetriq::NcursesDisplay::~NcursesDisplay()
{
    delwin(_window);
    endwin();
}

bool tetriq::NcursesDisplay::loadGame(const ITetris &game, uint64_t player_count)
{
    uint64_t board_width = (game.getWidth() + SIDEBAR_SIZE) * BLOCK_SIZE * 2;
    uint64_t board_height = game.getHeight() * BLOCK_SIZE * 2;
    uint64_t other_boards_width = player_count * game.getWidth() * BLOCK_SIZE;
    uint64_t width = board_width + other_boards_width;
    uint64_t height = board_height;

    // if (width > static_cast<uint64_t>(_win_width) || height > static_cast<uint64_t>(_win_height)) {
    //     Logger::log(LogLevel::ERROR, "Screen too small to display game");
    //     return false;
    // }

    return true;
}

bool tetriq::NcursesDisplay::draw(
    const Client &client, ITetrisIter otherGamesStart, ITetrisIter otherGamesEnd)
{
    erase();
    werase(_window);

    if (_show_help)
        displayHelp();

    ITetris &game = client.getGame();
    drawGame(game, {0, 0}, BLOCK_SIZE * 2, client.targetId == 0);
    drawCurrentTetromino(game);
    drawNextTetromino(game);
    drawPrediction(game);
    drawPowerUps(game);

    uint64_t index = 1;
    uint64_t x = (game.getWidth() + SIDEBAR_SIZE) * BLOCK_SIZE * 2;
    while (otherGamesStart != otherGamesEnd) {
        drawGame(**otherGamesStart, {x, 0}, BLOCK_SIZE, client.targetId == index);
        x += (*otherGamesStart)->getWidth() * BLOCK_SIZE;
        ++otherGamesStart;
        index++;
    }

    wrefresh(_window);
    return true;
}

bool tetriq::NcursesDisplay::handleEvents(Client &client)
{
    static bool is_shift_pressed = false;
    static std::string target;
    ITetris &game = client.getGame();
    int ch;

    while ((ch = getch()) != ERR) {
        switch (ch) {
            case 27:
                return false;
            case KEY_LEFT:
                game.handleGameAction(GameAction::MOVE_LEFT);
                continue;
            case KEY_RIGHT:
                game.handleGameAction(GameAction::MOVE_RIGHT);
                continue;
            case KEY_UP:
                game.handleGameAction(GameAction::ROTATE);
                continue;
            case KEY_DOWN:
                game.handleGameAction(GameAction::MOVE_DOWN);
                continue;
            case ' ':
                game.handleGameAction(GameAction::DROP);
                continue;
            case 'Q':
                client.sendPowerUp();
                continue;
            case 'H':
                _show_help = !_show_help;
                continue;
            case 'Z':
                client.targetId--;
                continue;
            case 'X':
                client.targetId++;
                continue;
            default:
                if (is_shift_pressed && ch >= '0' && ch <= '9') {
                    if (target.size() >= 3)
                        target.clear();
                    target += ch;
                    client.targetId = std::stoul(target);
                }
        }
        if (ch == 'S') {
            is_shift_pressed = !is_shift_pressed;
            if (is_shift_pressed) {
                target.clear();
                client.targetId = 0;
            }
        }
    }
    return true;
}

void tetriq::NcursesDisplay::drawGame(
    const ITetris &game, Position position, uint64_t block_size, bool is_target)
{
    BlockType blockType;
    Position pos;

    for (uint64_t x = 0; x < game.getWidth(); x++) {
        for (uint64_t y = 0; y < game.getHeight(); y++) {
            blockType = game.getBlockAt(x, y);
            pos = {position.x + x * block_size, position.y + y * block_size};
            drawBlock(pos, blockType, block_size, is_target);
        }
    }
}

void tetriq::NcursesDisplay::drawBlock(
    Position pos, BlockType blockType, uint64_t block_size, bool is_target)
{
    int color = COLOR_WHITE;
    char blockChar = '#';
    switch (blockType) {
        case BlockType::EMPTY:
            return;
        case BlockType::RED:
            color = COLOR_RED;
            break;
        case BlockType::BLUE:
        case BlockType::DARK_BLUE:
            color = COLOR_BLUE;
            break;
        case BlockType::ORANGE:
        case BlockType::YELLOW:
            color = COLOR_YELLOW;
            break;
        case BlockType::GREEN:
            color = COLOR_GREEN;
            break;
        case BlockType::PURPLE:
            color = COLOR_MAGENTA;
            break;
        case BlockType::INDESTRUCTIBLE:
            // color = is_target ? COLOR_WHITE : COLOR_BLACK;
            color = COLOR_WHITE;
            break;
        case BlockType::PU_ADD_LINE:
            color = COLOR_GREEN;
            blockChar = '+'; // "al"
            break;
        case BlockType::PU_GRAVITY:
            color = COLOR_MAGENTA;
            blockChar = 'g'; // "gv"
            break;
        case BlockType::PU_BLOCK_BOMB:
            color = COLOR_RED;
            blockChar = 'b'; // "bo"
            break;
        case BlockType::PU_CLEAR_LINE:
            color = COLOR_GREEN;
            blockChar = '-'; // "cl"
            break;
        case BlockType::PU_NUKE_FIELD:
            color = COLOR_RED;
            blockChar = 'n'; // "nu"
            break;
        case BlockType::PU_COLUMN_SHUFFLE:
            color = COLOR_YELLOW;
            blockChar = 'h'; // sh
            break;
        case BlockType::PU_SWITCH_FIELD:
            color = COLOR_CYAN;
            blockChar = 's'; // "sw"
            break;
        case BlockType::PU_CLEAR_BLOCK_RANDOM:
            color = COLOR_WHITE;
            blockChar = '?'; // "c?"
            break;
        case BlockType::PU_CLEAR_SPECIAL_BLOCK:
            color = COLOR_YELLOW;
            blockChar = 'o'; // "cs"
            break;
        default:
            color = COLOR_WHITE;
            break;
    }

    attron(COLOR_PAIR(color));
    for (uint64_t i = 0; i < block_size; i++) {
        for (uint64_t j = 0; j < block_size; j++) {
            mvaddch(pos.y + j, pos.x + i, blockChar);
        }
    }
    attroff(COLOR_PAIR(color));
}

void tetriq::NcursesDisplay::drawTetromino(
    const Tetromino &tetromino, Position position, uint64_t block_size)
{
    const TetroRotation &shape = tetromino.getTetroRotation();

    for (int i = 0; i < 4; i++) {
        std::tuple<char, char> local_pos = shape.at(i);
        unsigned int x = (position.x + std::get<0>(local_pos)) * block_size;
        unsigned int y = (position.y + std::get<1>(local_pos)) * block_size;
        drawBlock({x, y}, tetromino.getType(), block_size, false);
    }
}

void tetriq::NcursesDisplay::drawCurrentTetromino(const ITetris &game)
{
    drawTetromino(game.getCurrentPiece(), game.getCurrentPiece().getPosition(), BLOCK_SIZE * 2);
}

void tetriq::NcursesDisplay::drawNextTetromino(const ITetris &game)
{
    Position pos = {game.getWidth() + 1, 1};

    drawTetromino(game.getNextPiece(), pos, BLOCK_SIZE * 2);
}

void tetriq::NcursesDisplay::drawPrediction(const ITetris &game)
{
    const Tetromino &current = game.getCurrentPiece();
    Position pos = current.getPosition();
    const TetroRotation &shape = current.getTetroRotation();

    for (int i = 0; i < 4; i++) {
        std::tuple<char, char> local_pos = shape.at(i);
        unsigned int tempx = pos.x + std::get<0>(local_pos);
        unsigned int tempy = pos.y + std::get<1>(local_pos);
        while (tempy < game.getHeight() && game.getBlockAt(tempx, tempy) == BlockType::EMPTY) {
            mvaddch(tempy, tempx, '.');
            tempy++;
        }
    }
}

void tetriq::NcursesDisplay::drawPowerUps(const ITetris &game)
{}

void tetriq::NcursesDisplay::displayHelp()
{}
