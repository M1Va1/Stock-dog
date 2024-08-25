#include "debug.h"

int64_t perft(int depth, ChessBoard board, Color color) {
    if (depth == 0)
        return 1;
    board.GenAllMoves(color);
    if (depth == 1)
        return board.moves.size();
    int64_t nodes = 0;
    for (Move& cur_move : board.moves) {
        board.MakeMove(cur_move);
        nodes += perft(depth - 1, board, static_cast<Color>(!color));
        board.MakeMove(cur_move.Inversed());
    }
    return nodes;
}

void PrintMove(const Move cur_move) {
    std::cout << "from: " << cur_move.GetFrom() << "   to: " << cur_move.GetTo() << ' '
              << "    type: " << cur_move.GetType() << '\n';
}

void VisualizeMoves(const std::vector<Move>& moves, ChessBoard& board) {
    ChessBoard prev_board = board;
    for (auto move : moves) {
        board.MakeMove(move);
        board.PrintBoard();
        PrintMove(move);
        std::cout << "-----------\n";
        board = prev_board;
    }
}

void VisualizeSquares(std::vector<Square> squares) {
    ChessBoard b;
    for (auto sq : squares) {
        b.SetPiece(PAWN, WHITE, sq);
    }
    b.PrintBoard();
}

void VisualizeBitboard(Bitboard bb) {
    std::vector<Square> squares = GetSquares(bb);
    VisualizeSquares(squares);
}

void VisualizeBitboards(const std::vector<Bitboard>& arr) {
    for (auto cur_bb : arr) {
        VisualizeBitboard(cur_bb);
        std::cout << "-----------\n";
    }
}
