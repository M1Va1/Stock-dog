#pragma once
#include "basic_classes.h"
#include <fstream>
#include <iostream>

class MagicGenerator {
public:
    MagicGenerator();
    Bitboard GenRookMask(Square sq) const;
    Bitboard GenBishopMask(Square sq) const;
    void FillMasks();

    Bitboard GenRookMoves(Square sq, Bitboard occupied) const;
    Bitboard GenBishopMoves(Square sq, Bitboard occupied) const;
    std::vector<Bitboard> GenBlockerMasks(Bitboard mask) const;
    Bitboard FindRookMagic(Square sq);
    Bitboard FindBishopMagic(Square sq);
    void FillMagics();

    void PrintMagics() const;
    void PrintMasks() const;
    void PrintShifts() const;
    void PrintTables() const;

    void SaveTables(const std::string& rookFilename, const std::string& bishopFilename) const;
    void LoadTables(const std::string& rookFilename, const std::string& bishopFilename);

    // private:
    std::array<Bitboard, SQUARE_NB> rook_masks;
    std::array<Bitboard, SQUARE_NB> bishop_masks;
    std::array<Bitboard, SQUARE_NB> rook_magics;
    std::array<Bitboard, SQUARE_NB> bishop_magics;
    std::array<int, SQUARE_NB> rook_shifts;
    std::array<int, SQUARE_NB> bishop_shifts;
    std::array<std::vector<Bitboard>, SQUARE_NB> rook_move_table;
    std::array<std::vector<Bitboard>, SQUARE_NB> bishop_move_table;
};

void saveBishopMoveTable(const std::vector<std::vector<int>>& table, const std::string& filename);
