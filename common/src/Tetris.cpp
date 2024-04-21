/*
** EPITECH PROJECT, 2024
** tetriq
** File description:
** Tetris.cpp
*/

#include "Tetris.hpp"

tetriq::Block::Block(Tetris &tetris, BlockType type) : _tetris(tetris), _type(type)
{
}

void tetriq::Block::fromString(const std::string& string)
{
}

std::string tetriq::Block::toString()
{
    return std::to_string(_type);
}

tetriq::StandardBlock::StandardBlock(Tetris& _tetris, BlockType type) : Block(_tetris, type)
{
}

tetriq::StandardBlock::~StandardBlock()
= default;

bool tetriq::StandardBlock::isDestructible()
{
    return true;
}

tetriq::IndestructibleBlock::IndestructibleBlock(Tetris& _tetris) : Block(_tetris, BlockType::INDESTRUCTIBLE)
{
}

tetriq::IndestructibleBlock::~IndestructibleBlock()
= default;

bool tetriq::IndestructibleBlock::isDestructible()
{
    return false;
}

tetriq::SpecialBlock::SpecialBlock(Tetris& _tetris, BlockType type) : Block(_tetris, type)
{
}

tetriq::SpecialBlock::~SpecialBlock()
= default;

bool tetriq::SpecialBlock::isDestructible()
{
    return true;
}

tetriq::Tetris::Tetris(size_t width, size_t height)
{
    _width = width;
    _height = height;
    _blocks.resize(_height);
    for (size_t i = 0; i < _height; i++)
        _blocks[i].resize(_width);
    //fill blocks with standard blocks
    for (size_t i = 0; i < _height; i++)
        for (size_t j = 0; j < _width; j++)
            _blocks[i][j] = std::make_shared<StandardBlock>(*this, BlockType::EMPTY);
}

tetriq::Tetris::~Tetris()
{
    ;
}

std::string tetriq::Tetris::toString()
{
    return "";
}

void tetriq::Tetris::fromString(const std::string& string)
{
    ;
}
