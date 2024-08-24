#include "magic.h"
#include "move_generation.h"

int64_t perft(int depth, ChessBoard board);
void PrintPerftResults(int max_depth, ChessBoard board);
void VisualizeMoves(const std::vector<Move>& moves, ChessBoard& board);
void PrintMove(const Move cur_move);
void VisualizeSquares(std::vector<Square> squares);
void VisualizeBitboard(Bitboard bb);
void VisualizeBitboards(const std::vector<Bitboard>& arr);