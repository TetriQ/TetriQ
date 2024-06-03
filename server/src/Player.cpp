// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "Player.hpp"
#include "Channel.hpp"
#include "GameConfig.hpp"
#include "Logger.hpp"
#include "network/packets/FullGamePacket.hpp"
#include "network/packets/FullGameRequestPacket.hpp"
#include "network/packets/GameActionPacket.hpp"
#include "network/packets/InitGamePacket.hpp"
#include "network/packets/TickGamePacket.hpp"
#include <cassert>
#include <cstdint>
#include <string>

namespace tetriq {
    Player::Player(uint64_t network_id, ENetPeer *peer, Channel &channel)
        : _network_id(network_id)
        , _peer(peer)
        , _channel(channel)
        , _game(12, 22)
    {
        Logger::log(LogLevel::INFO, "player " + std::to_string(_network_id) + " connected.");
        // Network ids are unique so no way the player was already added
        assert(channel.addPlayer(*this));
    }

    Player::~Player()
    {
        _channel.removePlayer(*this);
        Logger::log(LogLevel::INFO, "player " + std::to_string(_network_id) + " disconnected.");
    }

    void Player::startGame(const GameConfig &config)
    {
        std::vector<uint64_t> other_players = _channel.getPlayers();
        other_players.erase(std::remove(other_players.begin(), other_players.end(), _network_id),
            other_players.end());
        InitGamePacket{config.width, config.height, _network_id, other_players}.send(_peer);
        FullGamePacket{_network_id, _game, 0}.send(_peer);
    }

    void Player::tickGame()
    {
        _game.tick();
        TickGamePacket{_applied_actions}.send(_peer);
        _applied_actions = 0;
    }

    void Player::applyPackets()
    {
        if (_game.isChanged()) {
            _channel.broadcastPacket(FullGamePacket{_network_id, _game, _applied_actions});
            _applied_actions = 0;
        }
    }

    uint64_t Player::getNetworkId() const
    {
        return _network_id;
    }

    void Player::sendPacket(const APacket &packet)
    {
        packet.send(_peer);
    }

    bool Player::handle(GameActionPacket &packet)
    {
        _game.handleGameAction(packet.getAction());
        _applied_actions++;
        return true;
    }

    bool Player::handle(FullGameRequestPacket &)
    {
        // TODO : its possible that this packets is handled in the middle of a
        // tick, we should wait for the end of the tick before sending info.
        FullGamePacket{_network_id, _game, _applied_actions}.send(_peer);
        _applied_actions = 0;
        return true;
    }

    bool Player::doPuSwitchField(BlockType power_up, Player &target)
    {
        if (power_up == BlockType::PU_SWITCH_FIELD) {
            Tetris &targetgame = target.getGame();
            // Don't Swap powerups
            std::deque<BlockType> tempPowerUps = _game.getPowerUps();
            _game.setPowerUps(targetgame.getPowerUps());
            targetgame.setPowerUps(tempPowerUps);
            // Swap boards
            Tetris tempBoard = _game;
            _game = targetgame;
            // Clear lines to make it fair
            for (uint64_t i = 0; i < 8; i++) {
                tempBoard.clearLine(i);
            }
            targetgame = tempBoard;
            LogLevel::DEBUG << "Player " << _network_id << " switched board with player "
                            << target.getNetworkId() << std::endl;
            return true;
        }
        return false;
    }

    bool Player::handle(PowerUpPacket &packet)
    {
        if (_game.isOver()) {
            return true;
        }
        BlockType power_up = _game.consumePowerUp();
        _game.setChanged(true);
        if (power_up == BlockType::EMPTY) {
            return true;
        }
        try {
            Player &target = _channel.getPlayerById(packet.getTarget());
            Tetris &target_game = target.getGame();
            if (target_game.isOver()) {
                return true;
            }
            if (doPuSwitchField(power_up, target))
                return true;
            target_game.applyPowerUp(power_up);
            target_game.setChanged(true);
            LogLevel::DEBUG << "Player " << _network_id << " applied "
                            << blockTypeToString(power_up) << " to player " << target.getNetworkId()
                            << std::endl;
        } catch (std::out_of_range &e) {
            LogLevel::ERROR << "Player not in channel" << std::endl;
        }
        return true;
    }

    Channel &Player::getChannel()
    {
        return _channel;
    }

    bool Player::isGameOver() const
    {
        return _game.isOver();
    }

    Tetris &Player::getGame()
    {
        return _game;
    }
}
