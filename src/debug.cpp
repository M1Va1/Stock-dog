#include "debug.h"

int64_t perft(int depth, int max_depth, ChessBoard& board, Color color, std::vector<int64_t>& types) {
    if (depth == 0) {
        if (max_depth == 1) {
            std::cout << board.last_move.GetUCImove() << ": " << 1 << '\n';
        }
        return 1;
    }
    board.GenAllMoves(color);
    // if (depth == 1)
    //     return board.moves.size();
    int64_t nodes = 0;
    for (Move& cur_move : board.moves) {
        ChessBoard buff = board;
        buff.MakeMove(cur_move);
        ++types[cur_move.GetType()];
        nodes += perft(depth - 1, max_depth, buff, static_cast<Color>(!color), types);
    }
    if ((depth + 1) == max_depth) {
        std::cout << board.last_move.GetUCImove() << ": " << nodes << '\n';
    }

    return nodes;
}

void PrintMove(const Move cur_move) {
    std::cout << "from: " << cur_move.GetFrom() << "   to: " << cur_move.GetTo() << ' '
              << "    type: " << cur_move.GetType() << '\n';
}

void PrintPerftResults(int max_depth, ChessBoard board) {
    std::vector<int64_t> types(4, 0);
    std::cout << perft(max_depth, max_depth, board, board.active_side, types) << '\n';
    std::cout << "en passants: " << types[2] << "\n\n";

    //     for (size_t i = 0; i <= max_depth; ++i) {
    //         std::vector<int64_t> types(4, 0);
    //         std::cout << "depth: " << i << ' ' << perft(i, i, board, board.active_side, types) << '\n';
    //         std::cout << "castlings: " << types[3] << '\n';
    //         std::cout << "promotions: " << types[1] << '\n';
    //         std::cout << "en passants: " << types[2] << "\n\n";
    //     }
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
