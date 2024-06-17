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
    : _tab(TabType::GAME)
{
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    _scr_width = getmaxx(stdscr);
    _scr_height = getmaxy(stdscr);
    int main_height = _scr_height - 5;
    _main_window = newwin(main_height, _scr_width, 0, 0);
    _menu_window = newwin(5, _scr_width, main_height, 0);

    start_color();
    init_pair(COLOR_BLACK, COLOR_BLACK, COLOR_WHITE);
    init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
    init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
}

tetriq::NcursesDisplay::~NcursesDisplay()
{
    delwin(_main_window);
    delwin(_menu_window);
    endwin();
}

bool tetriq::NcursesDisplay::loadGame(const ITetris &, uint64_t)
{
    // uint64_t board_width = (game.getWidth() + SIDEBAR_SIZE) * BLOCK_SIZE * 2;
    // uint64_t board_height = game.getHeight() * BLOCK_SIZE * 2;
    // uint64_t other_boards_width = player_count * game.getWidth() * BLOCK_SIZE;
    // uint64_t width = board_width + other_boards_width;
    // uint64_t height = board_height;

    tetriq::Logger::setLogVisibility(false);

    return true;
}

bool tetriq::NcursesDisplay::draw(
    const Client &client, ITetrisIter otherGamesStart, ITetrisIter otherGamesEnd)
{
    werase(_main_window);
    werase(_menu_window);

    box(_main_window, 0, 0);
    box(_menu_window, 0, 0);
    mvwprintw(_main_window, 0, 2, "TetriQ");

    mvwprintw(_menu_window, 1, 2, "F1: Game");
    mvwprintw(_menu_window, 2, 2, "F2: Chat");
    mvwprintw(_menu_window, 3, 2, "F3: Scoreboard");
    mvwprintw(_menu_window, 1, 20, "F4: Help");

    drawTab(client, otherGamesStart, otherGamesEnd);

    wrefresh(_main_window);
    wrefresh(_menu_window);
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
            case KEY_RESIZE:
                resizeWindow();
                continue;
            case KEY_F(1):
                _tab = TabType::GAME;
                continue;
            case KEY_F(2):
                _tab = TabType::CHAT;
                continue;
            case KEY_F(3):
                _tab = TabType::SCOREBOARD;
                continue;
            case KEY_F(4):
                _tab = TabType::HELP;
                continue;
            case 'H':
                _tab = TabType::HELP;
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

void tetriq::NcursesDisplay::drawBlock(Position pos, BlockType blockType, uint64_t block_size,
    bool is_target, short colorOverride, char charOverride)
{
    short blockColor;
    char blockChar = '#';
    switch (blockType) {
        case BlockType::EMPTY:
            return;
        case BlockType::RED:
            blockColor = COLOR_RED;
            break;
        case BlockType::BLUE:
        case BlockType::DARK_BLUE:
            blockColor = COLOR_BLUE;
            break;
        case BlockType::ORANGE:
        case BlockType::YELLOW:
            blockColor = COLOR_YELLOW;
            break;
        case BlockType::GREEN:
            blockColor = COLOR_GREEN;
            break;
        case BlockType::PURPLE:
            blockColor = COLOR_MAGENTA;
            break;
        case BlockType::INDESTRUCTIBLE:
            blockColor = is_target ? COLOR_WHITE : A_REVERSE;
            blockColor = COLOR_WHITE;
            break;
        case BlockType::PU_ADD_LINE:
            blockColor = COLOR_GREEN;
            blockChar = 'a'; // "al"
            break;
        case BlockType::PU_GRAVITY:
            blockColor = COLOR_MAGENTA;
            blockChar = 'g'; // "gv"
            break;
        case BlockType::PU_BLOCK_BOMB:
            blockColor = COLOR_RED;
            blockChar = 'b'; // "bo"
            break;
        case BlockType::PU_CLEAR_LINE:
            blockColor = COLOR_GREEN;
            blockChar = 'c'; // "cl"
            break;
        case BlockType::PU_NUKE_FIELD:
            blockColor = COLOR_RED;
            blockChar = 'n'; // "nu"
            break;
        case BlockType::PU_COLUMN_SHUFFLE:
            blockColor = COLOR_YELLOW;
            blockChar = 'h'; // sh
            break;
        case BlockType::PU_SWITCH_FIELD:
            blockColor = COLOR_CYAN;
            blockChar = 's'; // "sw"
            break;
        case BlockType::PU_CLEAR_BLOCK_RANDOM:
            blockColor = COLOR_WHITE;
            blockChar = '?'; // "c?"
            break;
        case BlockType::PU_CLEAR_SPECIAL_BLOCK:
            blockColor = COLOR_YELLOW;
            blockChar = 'o'; // "cs"
            break;
        default:
            blockColor = COLOR_WHITE;
            break;
    }

    if (colorOverride != ERR)
        blockColor = colorOverride;

    if (charOverride != ERR)
        blockChar = charOverride;

    wattron(_main_window, COLOR_PAIR(blockColor));
    for (uint64_t i = 0; i < block_size; i++) {
        for (uint64_t j = 0; j < block_size; j++) {
            mvwaddch(_main_window, pos.y + j, pos.x + i, blockChar);
        }
    }
    wattroff(_main_window, A_COLOR);
}

void tetriq::NcursesDisplay::drawTetromino(const Tetromino &tetromino, Position position,
    uint64_t block_size, short colorOverride, char charOverride)
{
    const TetroRotation &shape = tetromino.getTetroRotation();

    for (int i = 0; i < 4; i++) {
        std::tuple<char, char> local_pos = shape.at(i);
        unsigned int x = (position.x + std::get<0>(local_pos)) * block_size + 2; // +2 = offset x
        unsigned int y = (position.y + std::get<1>(local_pos)) * block_size + 1; // +1 = offset y
        drawBlock({x, y}, tetromino.getType(), block_size, false, colorOverride, charOverride);
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

void tetriq::NcursesDisplay::drawPrediction(ITetris &game)
{
    Tetromino predictedPiece = game.getCurrentPiece();

    predictedPiece.drop(game);

    wattron(_main_window, COLOR_PAIR(COLOR_WHITE));
    drawTetromino(predictedPiece, predictedPiece.getPosition(), BLOCK_SIZE * 2, COLOR_WHITE, '.');
    wattroff(_main_window, COLOR_PAIR(COLOR_WHITE));
}

void tetriq::NcursesDisplay::drawPowerUps(const ITetris &game)
{
    const auto it = game.getPowerUps().begin();
    const auto end = game.getPowerUps().end();

    mvwprintw(_main_window, (game.getHeight() + 1) * 2, 2, "Powerups: ");
    Position pos = {12, game.getHeight() + 1};
    for (auto i = it; i != end; ++i) {
        drawBlock({pos.x * BLOCK_SIZE, pos.y * BLOCK_SIZE * 2}, *i, BLOCK_SIZE, false);
        pos.x += 1;
    }
}

void tetriq::NcursesDisplay::drawTab(
    const Client &client, ITetrisIter otherGamesStart, ITetrisIter otherGamesEnd)
{
    switch (_tab) {
        case TabType::GAME:
            {
                mvwprintw(_menu_window, 0, 2, "Game");

                ITetris &game = client.getGame();
                drawGame(game, {2, 1}, BLOCK_SIZE * 2, client.targetId == 0);
                drawNextTetromino(game);
                drawPrediction(game);
                drawCurrentTetromino(game);
                drawPowerUps(game);

                uint64_t index = 1;
                uint64_t x = (game.getWidth() + SIDEBAR_SIZE) * BLOCK_SIZE * 2 + 2;
                while (otherGamesStart != otherGamesEnd) {
                    drawGame(**otherGamesStart, {x, 1}, BLOCK_SIZE, client.targetId == index);
                    x += (*otherGamesStart)->getWidth() * BLOCK_SIZE;
                    ++otherGamesStart;
                    index++;
                }
                break;
            }
        case TabType::CHAT:
            {
                mvwprintw(_menu_window, 0, 2, "Chat");
                break;
            }
        case TabType::SCOREBOARD:
            {
                mvwprintw(_menu_window, 0, 2, "Scoreboard");
                break;
            }
        case TabType::HELP:
            {
                mvwprintw(_menu_window, 0, 2, "Help");

                unsigned int y = 2;
                wattron(_main_window, COLOR_PAIR(COLOR_YELLOW));
                mvwprintw(_main_window, y, 2, "SPECIAL BLOCKS:");
                wattroff(_main_window, COLOR_PAIR(COLOR_YELLOW));
                y += 2;
                for (BlockType i = BlockType::PU_ADD_LINE; i != BlockType::PU_SWITCH_FIELD;
                     i = static_cast<BlockType>(static_cast<int>(i) + 1)) {
                    drawBlock({2, y}, i, BLOCK_SIZE, false);
                    mvwprintw(_main_window, y, 3, ": %s", blockTypeToString(i).c_str());
                    y++;
                }
                wattron(_main_window, COLOR_PAIR(COLOR_YELLOW));
                mvwprintw(_main_window, y += 2, 2, "CONTROLS:");
                wattroff(_main_window, COLOR_PAIR(COLOR_YELLOW));
                mvwprintw(_main_window, y += 2, 2, "Arrow keys: Move");
                mvwprintw(_main_window, y += 1, 2, "Up Arrow: Rotate");
                mvwprintw(_main_window, y += 1, 2, "Space: Drop");
                mvwprintw(_main_window, y += 2, 2, "Q: Send powerup");
                mvwprintw(_main_window, y += 1, 2, "S: Target player (HOLD)");
                mvwprintw(_main_window, y += 1, 2, "0-9: Target player id");

                break;
            }
    }
}

void tetriq::NcursesDisplay::resizeWindow()
{
    endwin();
    refresh();
    clear();
    _scr_width = getmaxx(stdscr);
    _scr_height = getmaxy(stdscr);
    int main_height = _scr_height - 5;
    _main_window = newwin(main_height, _scr_width, 0, 0);
    _menu_window = newwin(5, _scr_width, main_height, 0);
}
