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
    ChessBoard board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    board.PrintBoard();
    PrintPerftResults(5, board);
}   