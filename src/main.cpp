#include "magic.h"
#include "move_generation.h"
#include "debug.h"

int main() {
    // MagicGenerator mg;
    // mg.SaveTables("rook_move_table.dat", "bishop_move_table.dat");
    // mg.SaveMagics("rook_magics.dat", "bishop_magics.dat");
    LoadTablesGlobal("rook_move_table.dat", "bishop_move_table.dat");
    LoadMagicsGlobal("rook_magics.dat", "bishop_magics.dat");
    ChessBoard board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
    board.PrintBoard();
    board.GenAllMoves(BLACK);
    VisualizeMoves(board.moves, board);

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