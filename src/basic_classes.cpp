#include "basic_classes.h"

bool IsWithinBounds(Bitboard position, Direction dir) {
    if ((position & FILE_A) != 0) {
        if (dir == LEFT || dir == UP_LEFT || dir == DOWN_LEFT) {
            return false;
        }
    }
    if ((position & FILE_H) != 0) {
        if (dir == RIGHT || dir == UP_RIGHT || dir == DOWN_RIGHT) {
            return false;
        }
    }
    if ((position & RANK_8) != 0) {
        if (dir == UP || dir == UP_LEFT || dir == UP_RIGHT) {
            return false;
        }
    }
    if ((position & RANK_1) != 0) {
        if (dir == DOWN || dir == DOWN_LEFT || dir == DOWN_RIGHT) {
            return false;
        }
    }
    return true;
}

Bitboard MoveSquare(Bitboard bb, Direction dir) {
    if (!IsWithinBounds(bb, dir)) {
        return bb;
    }
    if (dir > 0) {
        return bb << dir;
    }
    return bb >> -dir;
}

Bitboard MoveSquare(Bitboard bb, const std::vector<Direction> &dirs) {
    Bitboard initial_bb = bb;
    for (Direction dir : dirs) {
        if (!IsWithinBounds(bb, dir)) {
            return initial_bb;
        }
        bb = MoveSquare(bb, dir);
    }
    return bb;
}

std::vector<Square> GetSquares(Bitboard bb) {
    std::vector<Square> squares;
    while (bb) {
        squares.push_back(static_cast<Square>(std::countr_zero(bb)));
        bb &= bb - 1;
    }
    return squares;
}

Bitboard GenRandomBitboard() {
    // https://stackoverflow.com/questions/5008804/generating-random-integer-from-a-range
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<Bitboard> dis(0, std::numeric_limits<Bitboard>::max());

    return dis(gen);
}

