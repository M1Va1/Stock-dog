#include <iostream>
#include <vector>
#include <string>

struct Move {
    int8_t row_start;
    int8_t column_start;
    int8_t row_end;
    int8_t column_end;
    // std::string UCI_Move;
};

struct Square {
    int8_t row;
    int8_t column;
    bool piece_existing = 0;
    Piece* piece;
};

class Board {
public:
    std::vector<std::vector<Square>> Field;
};

class Piece {
public:
    Board* board;
    bool color;
    std::vector<std::pair<int8_t, int8_t>> Deltas;  // row_delta, column_delta

    int8_t row;
    int8_t column;
    virtual std::vector<Move> GetAvailableMoves() {
    }
    virtual void DoMove() {
    }
};

class Pawn : public Piece {

    std::vector<Move> GetAvailableMoves() {
        std::vector<Move> ans;
        for (std::pair<int8_t, int8_t>& cur_delta : Deltas) {
            if (row + cur_delta.first >= 8 || column + cur_delta.second >= 8 || column + cur_delta.second <= 0 ||
                row + cur_delta.first <= 0) {
                continue;
            }
            Square* NextSquare = &board->Field[row + cur_delta.first][column + cur_delta.second];
            if (cur_delta.second == 0) {
                ans.push_back({row, column, NextSquare->row, NextSquare->column});
            } else if (NextSquare->piece_existing && NextSquare->piece->color != color) {
                ans.push_back({row, column, NextSquare->row, NextSquare->column});
            }
        }
        return ans;
    }
};

class Knight : public Piece {};

class Bishop : public Piece {};

class Queen : public Piece {};

class King : public Piece {};

class Rook : public Piece {};
