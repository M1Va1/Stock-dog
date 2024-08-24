#pragma once
#include "basic_classes.h"
#include "magic.h"
#include "move.h"
// #include "debug.h"


bool IsOccupied(Bitboard bb, Square sq);

void PrintKnightMasks();
void PrintKingMask();
Bitboard CalcMoveTable(Square sq, Bitboard block_board, PieceType pt, Color color = BLACK);

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
    Bitboard MoveToFriendSide(Bitboard bb);

    void MakeMove(Move cur_move);
    void GenPawnMoves(Color color);
    void GenKnightMoves(Color color);
    void GenBishopMoves(Color color);
    void GenRookMoves(Color color);
    void GenQueenMoves(Color color);
    void GenKingMoves(Color color);
    void ClearMoves();
    void GenPromotions(Square from, Square to);
    void GenAllMoves(Color color);

    void CalcAttackMap(Color color);
    void CalcCaptureMask(Color color);
    void CalcPushMask(Color color);
    void CalcAllMasks(Color color);

    void PrintBoard() const;

    bool IsDoublePush(Move move);
    bool IsInCheck(Color color);

    ChessBoard& operator=(const ChessBoard& rhs) = default;

    // private:
    std::vector<Move> moves;
    std::array<Bitboard, PIECE_NB> pieces;
    std::array<Bitboard, COLOR_NB> colors;
    std::array<Bitboard, COLOR_NB> attack_map;

    Bitboard capture_mask = 0xFFFFFFFFFFFFFFFFULL;
    Bitboard push_mask = 0xFFFFFFFFFFFFFFFFULL;

    Move last_move;
};

bool AvailableMove(Move move);

// std::vector<Move> GenLegalMoves(ChessBoard cur_board);
