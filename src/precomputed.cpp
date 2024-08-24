#include "precomputed.h"
namespace global {
std::array<std::vector<Bitboard>, SQUARE_NB> rook_move_table;
std::array<std::vector<Bitboard>, SQUARE_NB> bishop_move_table;
std::array<Bitboard, SQUARE_NB> rook_magics;
std::array<Bitboard, SQUARE_NB> bishop_magics;
}  // namespace global
