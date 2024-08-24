#include "debug.h"

// int64_t perft(int depth, ChessBoard board) {
//     if (depth == 0)
//         return 1;
//     std::vector<Move> legal_moves = GenLegalMoves(board);
//     int64_t nodes = 0;
//     for (Move& cur_move : legal_moves) {
//         board.MakeMove(cur_move);
//         nodes += perft(depth - 1, board);
//         board.MakeMove(cur_move.Inversed());
//     }
//     return nodes;
// }

// void PrintPerftResults(int max_depth, ChessBoard board) {
//     for (int cur_depth = 0; cur_depth <= max_depth; ++cur_depth) {
//         std::cout << cur_depth << ":   " << perft(cur_depth, board) << '\n';
//     }
// }

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
