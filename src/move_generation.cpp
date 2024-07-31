#include "move_generation.h"

void VisualizeSquares(std::vector<Square> squares) {
    ChessBoard b;
    for (auto sq : squares) {
        b.SetPiece(PAWN, WHITE, sq);
    }
    b.PrintBoard();
}

void ChessBoard::SetPiece(const PieceType pt, const Color c, const Square square) {
    Bitboard bb = SquareToBitboard(square);
    pieces[pt] |= bb;
    colors[c] |= bb;
    pieces[NONE] &= ~bb;
}

Piece ChessBoard::RemovePiece(Square square) {
    Bitboard bb = SquareToBitboard(square);
    Piece ans = PieceOnSquare(square);
    pieces[ans.type] &= ~bb;
    colors[ans.color] &= ~bb;
    pieces[NONE] |= bb;
    return ans;
}

bool IsOccupied(const Bitboard bb, const Square sq) {
    return ((bb & SquareToBitboard(sq)) >> sq);
}

Piece ChessBoard::PieceOnSquare(const Square sq) {
    uint8_t piece_type = 0;
    Color piece_color =
        static_cast<Color>(WHITE && IsOccupied(colors[WHITE], sq) || BLACK && IsOccupied(colors[BLACK], sq));

    for (uint8_t pt = 0; pt < PIECE_NB; ++pt)
        piece_type |= pt && IsOccupied(pieces[pt], sq);
    return {static_cast<PieceType>(piece_type), piece_color};
};

void ChessBoard::MakeMove(const Move cur_move) {
    Piece buff = RemovePiece(cur_move.GetFrom());
    RemovePiece(cur_move.GetTo());
    SetPiece(buff.type, buff.color, cur_move.GetTo());
}

ChessBoard FenEncoder(std::string input) {
    ChessBoard new_chessboard;
    size_t cur_ind = 0;
    for (int rank = 7; rank >= 0; --rank) {
        int file = 0;
        while (file < 8) {
            if (isdigit(input[cur_ind])) {
                file += (input[cur_ind] - '0');
                ++cur_ind;
            } else {
                new_chessboard.SetPiece(FenPieceCodes[tolower(input[cur_ind])], (Color)islower(input[cur_ind]),
                                        WhichSquare(rank, file));
                ++cur_ind;
                ++file;
            }
        }
        ++cur_ind;
    }
    return new_chessboard;
}
void Move::VisualizeMoves(std::vector<Move> &moves) {
    ChessBoard b;
    for (auto move : moves) {
        b.SetPiece(PAWN, WHITE, move.GetTo());
    }
    b.PrintBoard();
}

ChessBoard::ChessBoard() {
    colors.fill(0);
    pieces.fill(0);
    moves.clear();
    pieces[NONE] = -1;
}

ChessBoard::ChessBoard(const std::string &input) {
    ChessBoard buff = FenEncoder(input);
    colors = buff.colors;
    pieces = buff.pieces;
}

Bitboard ChessBoard::GetPieces(const Color color, const PieceType piece) const {
    return pieces[piece] & colors[color];
}

void ChessBoard::PrintBoard() const {
    std::array<std::array<char, 8>, 8> board;
    for (int i = 0; i < 8; ++i)
        board[i].fill('.');

    for (size_t pt = 0; pt < PIECE_NB; ++pt) {
        for (size_t sq = 0; sq < 64; ++sq) {
            Square square = static_cast<Square>(sq);
            if (pieces[pt] & SquareToBitboard(square)) {
                int row = sq / 8;
                int col = sq % 8;

                char piece_char = PieceLetters[static_cast<PieceType>(pt)];
                if (colors[WHITE] & SquareToBitboard(square)) {
                    piece_char = toupper(piece_char);
                }
                board[row][col] = piece_char;
            }
        }
    }
    for (int i = 7; i >= 0; --i) {
        std::cout << i + 1 << "  ";
        for (int j = 0; j < 8; ++j) {
            std::cout << board[i][j] << ' ';
        }
        std::cout << '\n';
    }
    std::cout << '\n';
    std::cout << "   ";
    for (int j = 0; j < 8; ++j) {
        std::cout << static_cast<char>('A' + j) << ' ';
    }
    std::cout << '\n';
}

void Move::SetFrom(const Square from) {
    description &= ~FROM_MASK;
    description |= (from << FROM_BITS);
}

void Move::SetTo(const Square to) {
    description &= ~TO_MASK;
    description |= (to << TO_BITS);
}

Square Move::GetFrom() const {
    return static_cast<Square>((description & FROM_MASK) >> FROM_BITS);
}

Square Move::GetTo() const {
    return static_cast<Square>((description & TO_MASK) >> TO_BITS);
}

Move::Move(Square from, Square to) {
    SetFrom(from);
    SetTo(to);
}

Move Move::Inversed() const {
    return Move(GetTo(), GetFrom());
}

void ChessBoard::GenPawnMoves(const Color color) {
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
}

void ChessBoard::GenKnightMoves(const Color color) {
    Bitboard knights = GetPieces(color, KNIGHT);
    Bitboard friendly_pieces = colors[color];

    for (Square from : GetSquares(knights)) {
        Bitboard possible_moves = 0;
        for (const auto &dir : KnightMoves) {
            possible_moves |= MoveSquare(SquareToBitboard(from), dir);
        }
        possible_moves &= ~friendly_pieces;
        for (Square to : GetSquares(possible_moves)) {
            moves.push_back({from, to});
        }
    }
}

void ChessBoard::GenBishopMoves(const Color color, const MagicGenerator &magic_generator) {
    Bitboard bishops = GetPieces(color, BISHOP);
    Bitboard board = ~GetEmptySquares();
    for (auto sq : GetSquares(bishops)) {
        Bitboard mask = bishop_masks[sq] & board;
        uint16_t index = (mask * magic_generator.bishop_magics[sq]) >> (magic_generator.bishop_shifts[sq]);
        std::vector<Square> bishop_moves = GetSquares(magic_generator.bishop_move_table[sq][index] & ~colors[color]);
        for (auto move : bishop_moves) {
            moves.push_back({sq, move});
        }
    }
}

void ChessBoard::GenRookMoves(const Color color, const MagicGenerator &magic_generator) {
    Bitboard rooks = GetPieces(color, ROOK);
    Bitboard board = ~GetEmptySquares();
    for (auto sq : GetSquares(rooks)) {
        Bitboard mask = rook_masks[sq] & board;
        uint16_t index = mask * magic_generator.rook_magics[sq] >> (magic_generator.rook_shifts[sq]);
        std::vector<Square> rook_moves = GetSquares(magic_generator.rook_move_table[sq][index] & ~colors[color]);
        for (auto move : rook_moves) {
            moves.push_back({sq, move});
        }
    }
}

void ChessBoard::GenQueenMoves(const Color color, const MagicGenerator &magic_generator) {
    Bitboard queens = GetPieces(color, QUEEN);
    Bitboard board = ~GetEmptySquares();
    for (auto sq : GetSquares(queens)) {
        Bitboard mask = bishop_masks[sq] & board;
        uint16_t index = (mask * magic_generator.bishop_magics[sq]) >> (magic_generator.bishop_shifts[sq]);
        std::vector<Square> bishop_moves = GetSquares(magic_generator.bishop_move_table[sq][index] & ~colors[color]);
        for (auto move : bishop_moves) {
            moves.push_back({sq, move});
        }
    }
    for (auto sq : GetSquares(queens)) {
        Bitboard mask = rook_masks[sq] & board;
        uint16_t index = mask * magic_generator.rook_magics[sq] >> (magic_generator.rook_shifts[sq]);
        std::vector<Square> rook_moves = GetSquares(magic_generator.rook_move_table[sq][index] & ~colors[color]);
        for (auto move : rook_moves) {
            moves.push_back({sq, move});
        }
    }
}
