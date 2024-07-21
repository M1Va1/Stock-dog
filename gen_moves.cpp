#include <basic_classes.h>

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

Bitboard MoveSquare(Bitboard bb, std::vector<Direction> dirs) {
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

std::vector<Move> ChessBoard::GenPawnMoves(const Color color) {
    std::vector<Move> moves;
    Bitboard pawns = GetPieces(color, PAWN);
    Bitboard empty_squares = GetEmptySquares();

    Direction dir = (color == WHITE) ? UP : DOWN;
    Rank reachable_rank = (color == WHITE) ? RANK_4 : RANK_5;

    Bitboard first_push = MoveSquare(pawns, dir) & empty_squares;
    for (Square to : GetSquares(first_push)) {
        Square from = static_cast<Square>(to - dir);
        moves.push_back({from, to});
    }

    Bitboard second_push = MoveSquare(first_push, dir) & empty_squares & reachable_rank;
    for (Square to : GetSquares(second_push)) {
        Square from = static_cast<Square>(to - 2 * dir);
        moves.push_back({from, to});
    }

    Bitboard left_attacks = MoveSquare(pawns, static_cast<Direction>(dir + LEFT)) & colors[!color];
    for (Square to : GetSquares(left_attacks)) {
        Square from = static_cast<Square>(to - dir - LEFT);
        moves.push_back({from, to});
    }

    Bitboard right_attacks = MoveSquare(pawns, static_cast<Direction>(dir + RIGHT)) & colors[!color];
    for (Square to : GetSquares(right_attacks)) {
        Square from = static_cast<Square>(to - dir - RIGHT);
        moves.push_back({from, to});
    }

    return moves;
}

std::vector<Move> ChessBoard::GenKnightMoves(const Color color) {
    std::vector<Move> moves;
    Bitboard knights = GetPieces(color, KNIGHT);
    Bitboard friendly_pieces = colors[color];

    for (Square from : GetSquares(knights)) {
        Bitboard possible_moves = 0;
        for (auto dir : KnightMoves) {
            possible_moves |= MoveSquare(SquareToBitboard(from), dir);
        }
        possible_moves &= ~friendly_pieces;
        for (Square to : GetSquares(possible_moves)) {
            moves.push_back({from, to});
        }
    }

    return moves;
}