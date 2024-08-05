#include "magic.h"
#include "move_generation.h"
#include "debug.h"

int main() {
    MagicGenerator mg;
    mg.SaveTables("rook_move_table.dat", "bishop_move_table.dat");
    mg.rook_move_table = {};
    mg.bishop_move_table = {};

    mg.LoadTables("rook_move_table.dat", "bishop_move_table.dat");
    mg.PrintTables();
    return 0;
}