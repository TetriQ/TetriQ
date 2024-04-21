/*
** EPITECH PROJECT, 2024
** tetriq
** File description:
** Tetris.hpp
*/

#ifndef TETRIS_H
#define TETRIS_H

#include "Network.hpp"
#include <vector>
#include <memory>

namespace tetriq
{
    class Tetris;

    enum BlockType {
        EMPTY,
        RED,
        BLUE,
        DARK_BLUE,
        ORANGE,
        YELLOW,
        GREEN,
        PURPLE,
        INDESTRUCTIBLE,
        SPECIAL
    };

    class Block : public NetworkObject {
        public:
            explicit Block(Tetris &tetris, BlockType type);
            ~Block() override = default;
            virtual bool isDestructible() = 0;
            void fromString(const std::string& string) override;
            std::string toString() override;
        private:
            Tetris &_tetris;
            BlockType _type;
    };

    class StandardBlock final : public Block {
        public:
            explicit StandardBlock(Tetris& _tetris, BlockType type);
            ~StandardBlock() override;
            bool isDestructible() override;
    };

    class IndestructibleBlock final: public Block {
        public:
            explicit IndestructibleBlock(Tetris& _tetris);
            ~IndestructibleBlock() override;
            bool isDestructible() override;
    };

    class SpecialBlock : public Block {
        public:
            explicit SpecialBlock(Tetris& _tetris, BlockType type);
            ~SpecialBlock() override;
            bool isDestructible() override;
    };

    class Tetris final: public NetworkObject {
        public:
            Tetris(size_t width, size_t height);
            ~Tetris() override;
            std::string toString() override;
            void fromString(const std::string &string) override;
        private:
            std::vector<std::vector<std::shared_ptr<Block>>> _blocks;
            size_t _width;
            size_t _height;
    };
}


#endif //TETRIS_H
