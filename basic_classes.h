#include <iostream>
#include <vector>
#include <string>

typedef uint64_t Bitboard;

// clang-format off
enum Square : uint8_t {
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8, 
};

enum PieceType : uint8_t {
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,

    PIECE_NB = 6
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

struct Move {
    Square from;
    Square to;
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

inline Bitboard squareToBitboard(int square) {
    return Bitboard(1) << square;
}

class ChessBoard {
public:
    Bitboard pieces[PIECE_NB];
    Bitboard colors[COLOR_NB];

    void setPiece(PieceType pt, Color c, Square square);

    void removePiece(PieceType pt, Color c, Square square);

    void printBoard();
};

