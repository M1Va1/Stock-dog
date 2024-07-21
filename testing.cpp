#include <basic_classes.h>

int64_t perft(int depth, ChessBoard board) {
    if (depth == 0)
        return 1;
    std::vector<Move> legal_moves = GenLegalMoves(board);
    int64_t nodes = 0;
    for (Move& cur_move : legal_moves) {
        board.MakeMove(cur_move);
        nodes += perft(depth - 1, board);
        board.MakeMove(cur_move.Inversed());
    }
    return nodes;
}

void PrintPerftResults(int max_depth, ChessBoard board) {
    for (int cur_depth = 0; cur_depth <= max_depth; ++cur_depth) {
        std::cout << cur_depth << ":   " << perft(cur_depth, board) << '\n';
    }
}

// int main() {
//     ChessBoard board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
//     board.MakeMove({E1, E4});
//     board.MakeMove({A2, A3});
//     board.MakeMove({B2, B7});
//     board.MakeMove({D2, D3});
//     board.MakeMove({G1, A8});
//     board.MakeMove({A1, B1});

//     auto moves = board.GenKnightMoves(WHITE);
//     std::cout << moves.size() << '\n';
//     for (auto move : moves) {
//         std::cout << SquareToString(move.GetFrom()) << ' ' << SquareToString(move.GetTo()) << '\n';
//     }

//     board.PrintBoard();
// }