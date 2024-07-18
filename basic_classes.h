#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <array>

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

inline Square WhichSquare(uint8_t rank, uint8_t file){
    return (Square)(rank * 8 + file);
}



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

std::map <char,PieceType> FenPiecesCodes =  {{'p', PAWN}, {'n', KNIGHT},{'b', BISHOP},{'r', ROOK},{'q', QUEEN},{'k', KING}};

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



inline Bitboard squareToBitboard(int square) {
    return Bitboard(1) << square;
}

enum MoveType {
    NORMAL,
    PROMOTION  = 1 << 14,
    EN_PASSANT = 2 << 14,
    CASTLING   = 3 << 14
};

struct Move{
    Square from;
    Square to;
};

bool IsTrueInSqure(const Bitboard& bb, const Square& sq);

bool AvailableMove(const Move& move);

class ChessBoard {
public:
    std::array<Bitboard, PIECE_NB> pieces;
    std::array<Bitboard, COLOR_NB> colors;

    ChessBoard(std::string input);
    
    ChessBoard();

    void setPiece(const PieceType& pt, const Color& c, const Square& square);

    Piece removePiece(Square& square);

    Piece PieceOnSquare(Square& sq);
    bool AvailableMove(Move& move);
    
    void DoMove(Move cur_move);

    void printBoard();

    ChessBoard operator = (const ChessBoard& rhs);
};



