#include "magic.h"
#include "move_generation.h"

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

int main() {
    ChessBoard b("r2qkbnr/pppppppp/pppppppp/4b3/4Q3/8/8/R2RK1NR");
    b.PrintBoard();
    MagicGenerator magic_gen;
    std::cout << "===============================" << std::endl;
    // magic_gen.PrintTables();
    b.GenQueenMoves(WHITE, magic_gen);
    Move::VisualizeMoves(b.moves);
    // magic_gen.PrintMagics();
    return 0;
}

// todo: precompute magics