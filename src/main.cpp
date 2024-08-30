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
    ChessBoard board("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 ");
    board.PrintBoard();
    PrintPerftResults(5, board);
}