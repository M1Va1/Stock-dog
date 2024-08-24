#include "move_generation.h"

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

void PrintKnightMasks() {
    for (int8_t from = A1; from <= H8; ++from) {
        if (from % 4 == 0) {
            std::cout << '\n';
        }
        Bitboard possible_moves = 0;
        for (const auto &dir : KnightMoves) {
            possible_moves |= MoveSquare(SquareToBitboard(static_cast<Square>(from)), dir);
        }
        possible_moves &= ~SquareToBitboard(static_cast<Square>(from));
        std::cout << possible_moves << "ULL, ";
    }
}
void PrintKingMask() {
    for (int8_t from = A1; from <= H8; ++from) {
        if (from % 4 == 0) {
            std::cout << '\n';
        }
        Bitboard possible_moves = 0;
        for (const auto &dir : {UP, DOWN, LEFT, RIGHT, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT}) {
            possible_moves |= MoveSquare(SquareToBitboard(static_cast<Square>(from)), dir);
        }
        possible_moves &= ~SquareToBitboard(static_cast<Square>(from));
        std::cout << possible_moves << "ULL, ";
    }
}

bool IsOccupied(const Bitboard bb, const Square sq) {
    return ((bb & SquareToBitboard(sq)) >> sq);
}

Piece ChessBoard::PieceOnSquare(const Square sq) {
    uint8_t piece_type = 0;
    Color piece_color =
        static_cast<Color>(WHITE && IsOccupied(colors[WHITE], sq) || BLACK && IsOccupied(colors[BLACK], sq));

    for (uint8_t pt = 0; pt < PIECE_NB; ++pt)
        piece_type |= (pt * IsOccupied(pieces[pt], sq));
    return {static_cast<PieceType>(piece_type), piece_color};
};

Bitboard CalcMoveTable(Square sq, Bitboard block_board, PieceType pt, Color color) {
    if (pt == BISHOP) {
        Bitboard mask = bishop_masks[sq] & block_board;
        uint16_t index = ((mask * global::bishop_magics[sq]) >> (bishop_shifts[sq]));
        return global::bishop_move_table[sq][index];
    } else if (pt == ROOK) {
        Bitboard mask = rook_masks[sq] & block_board;
        uint16_t index = ((mask * global::rook_magics[sq]) >> (rook_shifts[sq]));
        return global::rook_move_table[sq][index];
    } else if (pt == QUEEN) {
        Bitboard mask_of_bishop = bishop_masks[sq] & block_board;
        Bitboard mask_of_rook = rook_masks[sq] & block_board;
        uint16_t bishop_index = ((mask_of_bishop * global::bishop_magics[sq]) >> (bishop_shifts[sq]));
        uint16_t rook_index = ((mask_of_rook * global::rook_magics[sq]) >> (rook_shifts[sq]));
        return global::rook_move_table[sq][rook_index] & global::bishop_move_table[sq][bishop_index];
    } else if (pt == KNIGHT) {
        return knight_masks[sq];
    } else if (pt == PAWN) {
        Direction dir = (color == WHITE) ? UP : DOWN;
        return (MoveSquare(sq, (dir + LEFT)) | MoveSquare(sq, (dir + RIGHT)));
    } else {
        throw "WHRONG TYPE";
    }
}

void ChessBoard::MakeMove(const Move cur_move) {
    if (cur_move.GetType() == NORMAL || cur_move.GetType() == EN_PASSANT) {
        Piece buff = RemovePiece(cur_move.GetFrom());
        RemovePiece(cur_move.GetTo());
        SetPiece(buff.type, buff.color, cur_move.GetTo());
        if (cur_move.GetType() == EN_PASSANT) {
            RemovePiece(GetFirstSquare(MoveToFriendSide(cur_move.GetTo())));
        }
    } else if (cur_move.GetType() == PROMOTION) {
        Piece buff = RemovePiece(cur_move.GetFrom());
        RemovePiece(cur_move.GetTo());
        SetPiece(cur_move.GetPromoPieceType(), buff.color, cur_move.GetTo());
    } else {  // CASTLING
        ;
    }
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

bool ChessBoard::IsInCheck(Color color) {
    Square KingPos = GetFirstSquare(GetPieces(color, KING));
    Direction dir = (color == WHITE) ? UP : DOWN;
    for (Direction cur_dir : {static_cast<Direction>(dir + LEFT), static_cast<Direction>(dir + RIGHT)}) {
        Piece CheckingPiece = PieceOnSquare(MoveSquare(KingPos, cur_dir));
        if (CheckingPiece.color != color && CheckingPiece.type == PAWN)
            return true;
    }
    Bitboard board = ~GetEmptySquares();
    Color opposite_color = static_cast<Color>(BLACK - WHITE);
    Bitboard enemy_queens = GetPieces(opposite_color, QUEEN);
    Bitboard enemy_bishops = GetPieces(opposite_color, BISHOP);
    Bitboard enemy_rooks = GetPieces(opposite_color, ROOK);
    Bitboard bishop_available_moves = CalcMoveTable(KingPos, board, BISHOP);
    Bitboard rook_available_moves = CalcMoveTable(KingPos, board, ROOK);
    if (enemy_queens & bishop_available_moves || enemy_bishops & bishop_available_moves)
        return true;
    if (enemy_queens & rook_available_moves || enemy_rooks & rook_available_moves)
        return true;
    return false;
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

bool ChessBoard::IsDoublePush(Move move) {
    if (PieceOnSquare(move.GetFrom()).type == PAWN && abs(move.GetFrom() - move.GetTo()) == 2 * UP) {
        return true;
    }
    return false;
}

Bitboard ChessBoard::MoveToFriendSide(Bitboard bb) {
    if (PieceOnSquare(GetFirstSquare(bb)).color == WHITE)
        bb = MoveSquare(bb, DOWN);
    else
        bb = MoveSquare(bb, UP);
    return bb;
}

void ChessBoard::GenPromotions(Square from, Square to) {
    for (PieceType available_type : {KNIGHT, BISHOP, ROOK, QUEEN}) {
        moves.push_back({from, to, PROMOTION, available_type});
    }
}

void ChessBoard::GenPawnMoves(const Color color) {
    Bitboard pawns = GetPieces(color, PAWN);
    Bitboard empty_squares = GetEmptySquares();
    Bitboard DoublePushes = (IsDoublePush(last_move) ? SquareToBitboard(last_move.GetTo()) : 0);

    Direction dir = (color == WHITE) ? UP : DOWN;
    Rank reachable_rank = (color == WHITE) ? RANK_4 : RANK_5;

    Bitboard first_push = MoveSquare(pawns, dir) & empty_squares & push_mask;
    for (Square to : GetSquares(first_push)) {
        Square from = MoveSquare(to, -dir);
        if (SquareToBitboard(to) & RANK_1 || SquareToBitboard(to) & RANK_8) {
            GenPromotions(from, to);
        } else {
            moves.push_back({from, to});
        }
    }

    Bitboard second_push = MoveSquare(first_push, dir) & empty_squares & reachable_rank & push_mask;
    for (Square to : GetSquares(second_push)) {
        Square from = MoveSquare(to, -2 * dir);
        moves.push_back({from, to});
    }

    Bitboard left_attacks = MoveSquare(pawns, static_cast<Direction>(dir + LEFT)) & colors[!color] & capture_mask;
    for (Square to : GetSquares(left_attacks)) {
        Square from = MoveSquare(to, -dir - LEFT);
        if (SquareToBitboard(to) & RANK_1 || SquareToBitboard(to) & RANK_8) {
            GenPromotions(from, to);
        } else {
            moves.push_back({from, to});
        }
    }

    Bitboard right_attacks = MoveSquare(pawns, static_cast<Direction>(dir + RIGHT)) & colors[!color] & capture_mask;
    for (Square to : GetSquares(right_attacks)) {
        Square from = MoveSquare(to, -dir - RIGHT);
        if (SquareToBitboard(to) & RANK_1 || SquareToBitboard(to) & RANK_8) {
            GenPromotions(from, to);
        } else {
            moves.push_back({from, to});
        }
    }

    Bitboard left_en_passant = MoveSquare(pawns, static_cast<Direction>(dir + LEFT)) &
                               MoveToFriendSide(DoublePushes & colors[!color]) & push_mask &
                               MoveToFriendSide(capture_mask);
    if (left_en_passant) {
        Square to = GetFirstSquare(left_en_passant);
        Square from = MoveSquare(to, -dir - LEFT);
        moves.push_back({from, to, EN_PASSANT});
    }
    Bitboard right_en_passant = MoveSquare(pawns, static_cast<Direction>(dir + RIGHT)) &
                                MoveToFriendSide(DoublePushes & colors[!color] & push_mask) &
                                MoveToFriendSide(capture_mask);
    if (right_en_passant) {
        Square to = GetFirstSquare(right_en_passant);
        Square from = MoveSquare(to, -dir - RIGHT);
        moves.push_back({from, to, EN_PASSANT});
    }
}

void ChessBoard::GenKnightMoves(const Color color) {
    Bitboard knights = GetPieces(color, KNIGHT);
    Bitboard friendly_pieces = colors[color];

    for (Square from : GetSquares(knights)) {
        std::vector<Square> knight_moves =
            GetSquares(knight_masks[from] & ~friendly_pieces & (push_mask | capture_mask));

        for (Square to : knight_moves) {
            moves.push_back({from, to});
        }
    }
}

void ChessBoard::GenBishopMoves(const Color color) {
    Bitboard bishops = GetPieces(color, BISHOP);
    Bitboard board = ~GetEmptySquares();
    for (auto sq : GetSquares(bishops)) {
        std::vector<Square> bishop_moves =
            GetSquares(CalcMoveTable(sq, board, BISHOP) & ~colors[color] & (push_mask | capture_mask));
        for (auto move : bishop_moves) {
            moves.push_back({sq, move});
        }
    }
}

void ChessBoard::GenRookMoves(const Color color) {
    Bitboard rooks = GetPieces(color, ROOK);
    Bitboard board = ~GetEmptySquares();
    for (auto sq : GetSquares(rooks)) {
        std::vector<Square> rook_moves =
            GetSquares(CalcMoveTable(sq, board, ROOK) & ~colors[color] & (push_mask | capture_mask));
        for (auto move : rook_moves) {
            moves.push_back({sq, move});
        }
    }
}

void ChessBoard::GenQueenMoves(const Color color) {
    Bitboard queens = GetPieces(color, QUEEN);
    Bitboard board = ~GetEmptySquares();
    for (auto sq : GetSquares(queens)) {
        std::vector<Square> quen_moves =
            GetSquares(CalcMoveTable(sq, board, QUEEN) & ~colors[color] & (push_mask | capture_mask));
        for (auto move : quen_moves) {
            moves.push_back({sq, move});
        }
    }
}

void ChessBoard::GenKingMoves(Color color) {
    Square sq = GetFirstSquare(GetPieces(color, KING));
    if (sq == SQUARE_NB)
        return;
    Bitboard move_table = king_masks[sq] & ~colors[color] & ~attack_map[!color];
    std::vector<Square> king_moves = GetSquares(move_table);
    for (auto move : king_moves) {
        moves.push_back({sq, move});
    }
}

void ChessBoard::ClearMoves() {
    moves.clear();
}

void ChessBoard::GenAllMoves(const Color color) {
    ClearMoves();
    CalcAllMasks(color);
    if (capture_mask == 0) {
        GenKingMoves(color);
        return;
    }
    GenPawnMoves(color);
    GenKnightMoves(color);
    GenBishopMoves(color);
    GenRookMoves(color);
    GenQueenMoves(color);
    GenKingMoves(color);
}

void ChessBoard::CalcAttackMap(Color color) {
    Color opposite_color = static_cast<Color>(!color);
    Square king_pos = GetFirstSquare(GetPieces(opposite_color, KING));
    if (king_pos != SQUARE_NB)
        RemovePiece(king_pos);

    Bitboard board = ~GetEmptySquares();

    attack_map[color] = 0;
    for (PieceType pt : {PAWN, KNIGHT, BISHOP, ROOK, QUEEN}) {
        std::vector<Square> squares = GetSquares(GetPieces(color, pt));
        for (Square cur_sqre : squares) {
            attack_map[color] |= CalcMoveTable(cur_sqre, board, pt, color);
        }
    }
    if (king_pos != SQUARE_NB)
        SetPiece(KING, opposite_color, king_pos);
}

void ChessBoard::CalcCaptureMask(Color color) {
    Square king_pos = GetFirstSquare(GetPieces(color, KING));
    Color opposite_color = static_cast<Color>(!color);
    Direction dir = (color == WHITE) ? UP : DOWN;

    Bitboard board = ~GetEmptySquares();

    Bitboard enemy_pawns = GetPieces(opposite_color, PAWN);
    Bitboard enemy_queens = GetPieces(opposite_color, QUEEN);
    Bitboard enemy_bishops = GetPieces(opposite_color, BISHOP);
    Bitboard enemy_rooks = GetPieces(opposite_color, ROOK);
    Bitboard enemy_knights = GetPieces(opposite_color, KNIGHT);

    Bitboard pawns_available_moves = (MoveSquare(king_pos, (dir + LEFT)) | MoveSquare(king_pos, (dir + RIGHT)));
    Bitboard bishop_available_moves = CalcMoveTable(king_pos, board, BISHOP);
    Bitboard rook_available_moves = CalcMoveTable(king_pos, board, ROOK);
    Bitboard knight_available_moves = knight_masks[king_pos];

    capture_mask = enemy_pawns & pawns_available_moves | enemy_bishops & bishop_available_moves |
                   enemy_knights & knight_available_moves | enemy_queens & bishop_available_moves |
                   enemy_queens & rook_available_moves | enemy_rooks & rook_available_moves;
    if (!capture_mask)
        capture_mask = FULL_FIELD;
    std::vector<Square> attacked_by_squares = GetSquares(capture_mask);
    if (attacked_by_squares.size() == 2) {
        capture_mask = 0;
    }
}

void ChessBoard::CalcPushMask(Color color) {
    Square king_pos = GetFirstSquare(GetPieces(color, KING));
    push_mask = 0;
    Bitboard board = ~GetEmptySquares();
    Square attacked_by_square;
    Color opposite_color = static_cast<Color>(!color);
    if (capture_mask != FULL_FIELD)
        attacked_by_square = GetFirstSquare(capture_mask);
    else {
        push_mask = FULL_FIELD;
        return;
    }
    if (attacked_by_square == 64) {
        push_mask = 0;
        return;
    }
    PieceType cur_pt = PieceOnSquare(attacked_by_square).type;
    if (cur_pt == BISHOP || cur_pt == ROOK) {
        push_mask = CalcMoveTable(attacked_by_square, board, cur_pt) & CalcMoveTable(king_pos, board, cur_pt);
    } else if (cur_pt == QUEEN) {
        if (CalcMoveTable(king_pos, board, ROOK) & SquareToBitboard(attacked_by_square))
            push_mask = CalcMoveTable(attacked_by_square, board, ROOK) & CalcMoveTable(king_pos, board, ROOK);
        else
            push_mask = CalcMoveTable(attacked_by_square, board, BISHOP) & CalcMoveTable(king_pos, board, BISHOP);
    } else {
        push_mask = 0;
        return;
    }
}

void ChessBoard::CalcAllMasks(Color color) {
    Color opposite_color = static_cast<Color>(!color);
    CalcAttackMap(opposite_color);
    CalcCaptureMask(color);
    CalcPushMask(color);
}
