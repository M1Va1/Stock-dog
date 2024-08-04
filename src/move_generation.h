#pragma once
#include "basic_classes.h"
#include "precomputed.h"
#include "magic.h"
#include "move.h"

bool IsOccupied(Bitboard bb, Square sq);

class ChessBoard {
public:
    ChessBoard(const std::string& input);
    ChessBoard();

    void SetPiece(PieceType pt, Color c, Square square);
    Piece RemovePiece(Square square);
    Piece PieceOnSquare(Square sq);
    Bitboard GetPieces(Color color, PieceType piece) const;
    Bitboard GetEmptySquares() const {
        return pieces[NONE];
    };

    void MakeMove(Move cur_move);
    void GenPawnMoves(Color color);
    void GenKnightMoves(Color color);
    void GenBishopMoves(Color color, const MagicGenerator& magic_generator);
    void GenRookMoves(Color color, const MagicGenerator& magic_generator);
    void GenQueenMoves(Color color, const MagicGenerator& magic_generator);

    void PrintBoard() const;

    ChessBoard& operator=(const ChessBoard& rhs) = default;

    // private:
    std::vector<Move> moves;
    std::array<Bitboard, PIECE_NB> pieces;
    std::array<Bitboard, COLOR_NB> colors;
};

bool AvailableMove(Move move);

std::vector<Move> GenLegalMoves(ChessBoard cur_board); 
