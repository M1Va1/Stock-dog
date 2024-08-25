#include "magic.h"
#include "move_generation.h"
#include "debug.h"

int main() {
    // MagicGenerator mg;
    // mg.SaveTables("rook_move_table.dat", "bishop_move_table.dat");
    // mg.SaveMagics("rook_magics.dat", "bishop_magics.dat");
    LoadTablesGlobal("precomputed_info/rook_move_table.dat", "precomputed_info/bishop_move_table.dat");
    LoadMagicsGlobal("precomputed_info/rook_magics.dat", "precomputed_info/bishop_magics.dat");
    // PrintBishopMasks();
    // PrintRookMasks();
    ChessBoard board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    board.PrintBoard();
    for (int i = 0; i < 5; ++i) {
        std::cout << "depth " << i << ":    " << perft(i, board, WHITE) << '\n';
    }

    // board.GenAllMoves(WHITE);
    // std::cout << "Pinned Pieces:   " << board.pinned_pieces << '\n';
    // VisualizeMoves(board.moves, board);

    // MagicGenerator mg;
    // mg.SaveTables("rook_move_table.dat", "bishop_move_table.dat");
    // mg.rook_move_table_ = {};
    // mg.bishop_move_table_ = {};
    // mg.LoadTables("rook_move_table.dat", "bishop_move_table.dat");
    // mg.PrintTables();
    // PrintKnightMasks();
    // for(Bitboard cur_bb : rays[5]){
    //     VisualizeBitboard(cur_bb);
    //     std::cout << "----------\n";
    // }
    // std::cout << "\n\n\n";
    // PrintKingMask();
    // std::cout << "Hello worold!";
    // PrintRayMasks();
}