#include "magic.h"
#include "move_generation.h"
#include "debug.h"

int main() {
    ChessBoard b("r2qkbnr/pppppppp/pppppppp/4b3/4Q3/8/8/R2RK1NR");
    b.PrintBoard();
    MagicGenerator magic_gen;
    std::cout << "===============================" << std::endl;
    // magic_gen.PrintTables();
    b.GenQueenMoves(WHITE, magic_gen);
    VisualizeMoves(b.moves);
    // magic_gen.PrintMagics();
    return 0;
}