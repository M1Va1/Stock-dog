#pragma once

#include <array>
#include <iostream>
#include <map>
#include <format>
#include <string>
#include <vector>
#include <random>

using Bitboard = uint64_t;

constexpr uint8_t FROM_BITS = 10;
constexpr uint8_t TO_BITS = 4;

// clang-format off
enum Square : uint16_t {
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8, 

    SQUARE_NB = 64
};
// clang-format on

enum PieceType : uint8_t {
    NONE,
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,

    PIECE_NB = 7
};

enum Color : uint8_t {
    WHITE,
    BLACK,

    COLOR_NB = 2
};

struct Piece {
    PieceType type;
    Color color;
};

enum Direction : int8_t {
    UP = 8,
    DOWN = -8,
    LEFT = -1,
    RIGHT = 1,

    UP_LEFT = 7,
    UP_RIGHT = 9,
    DOWN_LEFT = -9,
    DOWN_RIGHT = -7
};

enum Rank : Bitboard {
    RANK_1 = 0b11111111ULL,
    RANK_2 = RANK_1 << UP,
    RANK_3 = RANK_2 << UP,
    RANK_4 = RANK_3 << UP,
    RANK_5 = RANK_4 << UP,
    RANK_6 = RANK_5 << UP,
    RANK_7 = RANK_6 << UP,
    RANK_8 = RANK_7 << UP
};

enum File : Bitboard {
    FILE_A = 0x0101010101010101ULL,
    FILE_B = FILE_A << 1,
    FILE_C = FILE_A << 2,
    FILE_D = FILE_A << 3,
    FILE_E = FILE_A << 4,
    FILE_F = FILE_A << 5,
    FILE_G = FILE_A << 6,
    FILE_H = FILE_A << 7
};

enum MoveType : uint16_t {
    NORMAL,

    PROMOTION = 1 << 14,
    EN_PASSANT = 2 << 14,
    CASTLING = 3 << 14
};

enum MoveMask : uint16_t {
    FROM_MASK = 0b1111110000000000,
    TO_MASK = 0b0000001111110000,
};

inline Square WhichSquare(uint8_t rank, uint8_t file) {
    return (Square)(rank * 8 + file);
}

inline std::map<char, PieceType> FenPieceCodes = {{'p', PAWN}, {'n', KNIGHT}, {'b', BISHOP},
                                                  {'r', ROOK}, {'q', QUEEN},  {'k', KING}};
inline std::map<PieceType, char> PieceLetters = {{NONE, '.'}, {PAWN, 'p'},  {KNIGHT, 'n'}, {BISHOP, 'b'},
                                                 {ROOK, 'r'}, {QUEEN, 'q'}, {KING, 'k'}};
inline std::map<int, char> FileLetters = {{0, 'A'}, {1, 'B'}, {2, 'C'}, {3, 'D'},
                                          {4, 'E'}, {5, 'F'}, {6, 'G'}, {7, 'H'}};

inline std::array<std::vector<Direction>, 8> KnightMoves = {{{UP, UP, LEFT},
                                                             {UP, UP, RIGHT},
                                                             {RIGHT, RIGHT, UP},
                                                             {RIGHT, RIGHT, DOWN},
                                                             {DOWN, DOWN, RIGHT},
                                                             {DOWN, DOWN, LEFT},
                                                             {LEFT, LEFT, DOWN},
                                                             {LEFT, LEFT, UP}}};

inline std::string SquareToString(Square sq) {
    return std::format("{}{}", FileLetters[sq % 8], sq / 8 + 1);
}

inline Bitboard SquareToBitboard(const Square square) {
    return Bitboard(1) << square;
}

bool IsWithinBounds(Bitboard position, Direction dir);

Bitboard MoveSquare(Bitboard bb, Direction dir);

Bitboard MoveSquare(Bitboard bb, const std::vector<Direction> &dirs);

std::vector<Square> GetSquares(Bitboard bb);

Bitboard GenRandomBitboard();



