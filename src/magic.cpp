#include "magic.h"

Bitboard MagicGenerator::GenRookMask(Square sq) const {
    Bitboard mask = 0;
    for (Direction dir : {UP, DOWN, LEFT, RIGHT}) {
        Bitboard bb = SquareToBitboard(sq);
        while (IsWithinBounds(bb, dir)) {
            bb = MoveSquare(bb, dir);
            if (IsWithinBounds(bb, dir)) {
                mask |= bb;
            }
        }
    }

    return mask;
}

Bitboard MagicGenerator::GenBishopMask(Square sq) const {
    Bitboard mask = 0;
    for (Direction dir : {UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT}) {
        Bitboard bb = SquareToBitboard(sq);
        while (IsWithinBounds(bb, dir)) {
            bb = MoveSquare(bb, dir);
            if (IsWithinBounds(bb, dir)) {
                mask |= bb;
            }
        }
    }

    return mask;
}

void MagicGenerator::FillMasks() {
    for (int sq = 0; sq < SQUARE_NB; ++sq) {
        rook_masks[sq] = GenRookMask(static_cast<Square>(sq));
        bishop_masks[sq] = GenBishopMask(static_cast<Square>(sq));
    }
}

Bitboard MagicGenerator::GenRookMoves(Square sq, Bitboard occupied) const {
    Bitboard moves = 0;
    for (Direction dir : {UP, DOWN, LEFT, RIGHT}) {
        Bitboard bb = SquareToBitboard(sq);
        while (IsWithinBounds(bb, dir)) {
            bb = MoveSquare(bb, dir);
            moves |= bb;
            if (occupied & bb)
                break;
        }
    }

    return moves;
}

Bitboard MagicGenerator::GenBishopMoves(Square sq, Bitboard occupied) const {
    Bitboard moves = 0;
    for (Direction dir : {UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT}) {
        Bitboard bb = SquareToBitboard(sq);
        while (IsWithinBounds(bb, dir)) {
            bb = MoveSquare(bb, dir);
            moves |= bb;
            if (occupied & bb)
                break;
        }
    }

    return moves;
}

std::vector<Bitboard> MagicGenerator::GenBlockerMasks(Bitboard mask) const {
    std::vector<Bitboard> masks;
    int num_bits = std::popcount(mask);
    int total_masks = 1 << num_bits;

    for (int i = 0; i < total_masks; ++i) {
        Bitboard bb_mask = 0;
        int bit_index = 0;
        for (int j = 0; j < num_bits; ++j) {
            while ((mask & (1ULL << bit_index)) == 0)
                ++bit_index;

            if (i & (1 << j))
                bb_mask |= (1ULL << bit_index);
            ++bit_index;
        }
        masks.push_back(bb_mask);
    }

    return masks;
}

Bitboard MagicGenerator::FindRookMagic(Square sq) {
    Bitboard mask = rook_masks[sq];
    std::vector<Bitboard> blocker_masks = GenBlockerMasks(mask);
    int relevant_bits = std::popcount(mask);

    int bits_to_shift = 64 - relevant_bits;
    while (true) {
        Bitboard magic_candidate = GenRandomBitboard() & GenRandomBitboard() & GenRandomBitboard();  //
        std::vector<Bitboard> move_table(1 << relevant_bits, 0);

        bool fail = false;
        for (Bitboard blocker_mask : blocker_masks) {
            uint16_t index = (blocker_mask * magic_candidate) >> bits_to_shift;
            Bitboard moves = GenRookMoves(sq, blocker_mask);

            if (move_table[index] == 0) {
                move_table[index] = moves;
            } else if (move_table[index] != moves) {
                fail = true;
                break;
            }
        }

        if (!fail) {
            rook_magics[sq] = magic_candidate;
            rook_shifts[sq] = bits_to_shift;
            rook_move_table[sq] = move_table;
            return magic_candidate;
        }
    }
}

Bitboard MagicGenerator::FindBishopMagic(Square sq) {
    Bitboard mask = bishop_masks[sq];
    std::vector<Bitboard> blocker_masks = GenBlockerMasks(mask);
    int relevant_bits = std::popcount(mask);
    int bits_to_shift = 64 - relevant_bits;
    while (true) {
        Bitboard magic_candidate = GenRandomBitboard() & GenRandomBitboard() & GenRandomBitboard();  //
        std::vector<Bitboard> move_table(1 << relevant_bits, 0);

        bool fail = false;
        for (Bitboard blocker_mask : blocker_masks) {
            uint16_t index = (blocker_mask * magic_candidate) >> bits_to_shift;
            Bitboard moves = GenBishopMoves(sq, blocker_mask);

            if (move_table[index] == 0) {
                move_table[index] = moves;
            } else if (move_table[index] != moves) {
                fail = true;
                break;
            }
        }

        if (!fail) {
            bishop_magics[sq] = magic_candidate;
            bishop_shifts[sq] = bits_to_shift;
            bishop_move_table[sq] = move_table;

            return magic_candidate;
        }
    }
}

void MagicGenerator::FillMagics() {
    for (int i = 0; i < SQUARE_NB; ++i) {
        Square sq = static_cast<Square>(i);
        rook_magics[sq] = FindRookMagic(sq);
        bishop_magics[sq] = FindBishopMagic(sq);
    }
}

void MagicGenerator::PrintMagics() const {
    std::cout << "Rook magics:\n";
    for (int i = 0; i < SQUARE_NB; ++i) {
        Square sq = static_cast<Square>(i);
        std::cout << rook_magics[sq] << "ULL,\n";
    }

    std::cout << "Bishop magics:\n";
    for (int i = 0; i < SQUARE_NB; ++i) {
        Square sq = static_cast<Square>(i);
        std::cout << bishop_magics[sq] << "ULL,\n";
    }
}

void MagicGenerator::PrintMasks() const {
    std::cout << "Rook masks:\n";
    for (int i = 0; i < SQUARE_NB; ++i) {
        Square sq = static_cast<Square>(i);
        std::cout << rook_masks[sq] << "ULL,\n";
    }

    std::cout << "Bishop masks:\n";
    for (int i = 0; i < SQUARE_NB; ++i) {
        Square sq = static_cast<Square>(i);
        std::cout << bishop_masks[sq] << "ULL,\n";
    }
}

void MagicGenerator::PrintShifts() const {
    std::cout << "Rook shifts:\n";
    for (int i = 0; i < SQUARE_NB; ++i) {
        Square sq = static_cast<Square>(i);
        std::cout << static_cast<int>(rook_shifts[sq]) << ",\n";
    }

    std::cout << "Bishop shifts:\n";
    for (int i = 0; i < SQUARE_NB; ++i) {
        Square sq = static_cast<Square>(i);
        std::cout << static_cast<int>(bishop_shifts[sq]) << ",\n";
    }
}

void MagicGenerator::PrintTables() const {
    std::cout << "Rook move tables:\n";
    for (int i = 0; i < SQUARE_NB; ++i) {
        Square sq = static_cast<Square>(i);
        std::cout << "{\n";
        for (auto rook_move : rook_move_table[sq]) {
            std::cout << rook_move << "ULL, \n";
        }
        std::cout << "},\n";
    }

    std::cout << "Bishop move tables:\n";
    for (int i = 0; i < SQUARE_NB; ++i) {
        Square sq = static_cast<Square>(i);
        std::cout << "{\n";
        for (auto bishop_move : bishop_move_table[sq]) {
            std::cout << bishop_move << "ULL, \n";
        }
        std::cout << "},\n";
    }
}

MagicGenerator::MagicGenerator() {
    FillMasks();
    FillMagics();
}

void MagicGenerator::SaveTables(const std::string& rookFilename, const std::string& bishopFilename) const {
    std::ofstream rookFile(rookFilename, std::ios::binary);
    if (!rookFile) {
        std::cerr << "Error opening file for writing: " << rookFilename << "\n";
        return;
    }

    for (const auto& table : rook_move_table) {
        size_t size = table.size();
        rookFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
        rookFile.write(reinterpret_cast<const char*>(table.data()), size * sizeof(Bitboard));
    }

    std::ofstream bishopFile(bishopFilename, std::ios::binary);
    if (!bishopFile) {
        std::cerr << "Error opening file for writing: " << bishopFilename << "\n";
        return;
    }

    for (const auto& table : bishop_move_table) {
        size_t size = table.size();
        bishopFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
        bishopFile.write(reinterpret_cast<const char*>(table.data()), size * sizeof(Bitboard));
    }
}

void MagicGenerator::LoadTables(const std::string& rookFilename, const std::string& bishopFilename) {
    std::ifstream rookFile(rookFilename, std::ios::binary);
    if (!rookFile) {
        std::cerr << "Error opening file for reading: " << rookFilename << "\n";
        return;
    }

    for (auto& table : rook_move_table) {
        size_t size;
        rookFile.read(reinterpret_cast<char*>(&size), sizeof(size));
        table.resize(size);
        rookFile.read(reinterpret_cast<char*>(table.data()), size * sizeof(Bitboard));
    }

    std::ifstream bishopFile(bishopFilename, std::ios::binary);
    if (!bishopFile) {
        std::cerr << "Error opening file for reading: " << bishopFilename << "\n";
        return;
    }

    for (auto& table : bishop_move_table) {
        size_t size;
        bishopFile.read(reinterpret_cast<char*>(&size), sizeof(size));
        table.resize(size);
        bishopFile.read(reinterpret_cast<char*>(table.data()), size * sizeof(Bitboard));
    }
}
