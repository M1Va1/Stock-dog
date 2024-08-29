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
    if (sq == SQUARE_NB)
        return 0;
    if (pt == BISHOP) {
        Bitboard mask = global::bishop_masks_for_magics[sq] & block_board;
        uint16_t index = ((mask * global::bishop_magics[sq]) >> (bishop_shifts[sq]));
        return global::bishop_move_table[sq][index];
    } else if (pt == ROOK) {
        Bitboard mask = global::rook_masks_for_magics[sq] & block_board;
        uint16_t index = ((mask * global::rook_magics[sq]) >> (rook_shifts[sq]));
        return global::rook_move_table[sq][index];
    } else if (pt == QUEEN) {
        Bitboard mask_of_bishop = global::bishop_masks_for_magics[sq] & block_board;
        Bitboard mask_of_rook = global::rook_masks_for_magics[sq] & block_board;
        uint16_t bishop_index = ((mask_of_bishop * global::bishop_magics[sq]) >> (bishop_shifts[sq]));
        uint16_t rook_index = ((mask_of_rook * global::rook_magics[sq]) >> (rook_shifts[sq]));
        return global::rook_move_table[sq][rook_index] | global::bishop_move_table[sq][bishop_index];
    } else if (pt == KNIGHT) {
        return knight_masks[sq];
    } else if (pt == PAWN) {
        Direction dir = (color == WHITE) ? UP : DOWN;
        return (SquareToBitboard(MoveSquare(sq, (dir + LEFT))) | SquareToBitboard(MoveSquare(sq, (dir + RIGHT))));
    } else if (pt == KING) {
        return king_masks[sq];
    } else {
        throw "WHRONG TYPE";
    }
}

bool ShortOrLongCastling(const Move castling) {
    if (castling.GetTo() == G1 || castling.GetTo() == G8)
        return 1;
    return 0;
}

void ChessBoard::MakeMove(const Move cur_move) {
    en_passant_squares = {0, 0};
    if (cur_move.GetType() == NORMAL || cur_move.GetType() == EN_PASSANT) {
        Piece buff;
        if (IsDoublePush(cur_move)) {
            buff = RemovePiece(cur_move.GetFrom());
            Direction dir = (buff.color == WHITE) ? UP : DOWN;
            en_passant_squares[!buff.color] = SquareToBitboard(MoveSquare(cur_move.GetFrom(), dir));
        } else
            buff = RemovePiece(cur_move.GetFrom());
        if (buff.type == KING)
            castling_availables[buff.color] = {0, 0};
        else if (buff.type == ROOK) {
            if (cur_move.GetFrom() == A1)
                castling_availables[0][0] = 0;
            else if (cur_move.GetFrom() == H1)
                castling_availables[0][1] = 0;
            else if (cur_move.GetFrom() == A8)
                castling_availables[1][0] = 0;
            else if (cur_move.GetFrom() == H8)
                castling_availables[1][1] = 0;
        }
        Piece eaten_piece = RemovePiece(cur_move.GetTo());
        if (eaten_piece.type == ROOK) {
            if (cur_move.GetTo() == A1 && eaten_piece.color == WHITE)
                castling_availables[0][0] = 0;
            else if (cur_move.GetTo() == H1 && eaten_piece.color == WHITE)
                castling_availables[0][1] = 0;
            else if (cur_move.GetTo() == A8 && eaten_piece.color == BLACK)
                castling_availables[1][0] = 0;
            else if (cur_move.GetTo() == H8 && eaten_piece.color == BLACK)
                castling_availables[1][1] = 0;
        }
        SetPiece(buff.type, buff.color, cur_move.GetTo());
        if (cur_move.GetType() == EN_PASSANT) {
            Direction op_dir = (buff.color == WHITE) ? DOWN : UP;
            RemovePiece(MoveSquare(cur_move.GetTo(), op_dir));
        }
    } else if (cur_move.GetType() == PROMOTION) {
        Piece buff = RemovePiece(cur_move.GetFrom());
        Piece eaten_piece = RemovePiece(cur_move.GetTo());
        if (eaten_piece.type == ROOK) {
            if (cur_move.GetTo() == A1 && eaten_piece.color == WHITE)
                castling_availables[0][0] = 0;
            else if (cur_move.GetTo() == H1 && eaten_piece.color == WHITE)
                castling_availables[0][1] = 0;
            else if (cur_move.GetTo() == A8 && eaten_piece.color == BLACK)
                castling_availables[1][0] = 0;
            else if (cur_move.GetTo() == H8 && eaten_piece.color == BLACK)
                castling_availables[1][1] = 0;
        }
        SetPiece(cur_move.GetPromoPieceType(), buff.color, cur_move.GetTo());
    } else if (cur_move.GetType() == CASTLING) {
        bool castle_ind = ShortOrLongCastling(cur_move);
        Piece king_buff = RemovePiece(cur_move.GetFrom());
        Piece rook_buff = RemovePiece(rook_start_positions[king_buff.color][castle_ind]);
        castling_availables[king_buff.color] = {0, 0};
        SetPiece(king_buff.type, king_buff.color, cur_move.GetTo());
        SetPiece(rook_buff.type, rook_buff.color, rook_after_castling[king_buff.color][castle_ind]);
    }
    penultimate_move = last_move = cur_move;
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
    if (input[cur_ind] == 'w')
        new_chessboard.active_side = WHITE;
    else
        new_chessboard.active_side = BLACK;
    cur_ind += 2;
    while (cur_ind < input.size() && input[cur_ind] != ' ') {
        if (input[cur_ind] == '-') {
            ++cur_ind;
            break;
        }
        bool castle_color = islower(input[cur_ind]);
        bool castle_side = (tolower(input[cur_ind]) == 'k');
        new_chessboard.castling_availables[castle_color][castle_side] = 1;
        ++cur_ind;
    }
    ++cur_ind;
    new_chessboard.en_passant_squares = {0, 0};
    if (cur_ind < input.size() && isalpha(input[cur_ind])) {
        new_chessboard.en_passant_squares[new_chessboard.active_side] =
            (FILES[input[cur_ind] - 'a'] & RANKS[input[cur_ind + 1] - '1']);
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
    *this = buff;
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
    if (PieceOnSquare(move.GetFrom()).type == PAWN && abs(move.GetFrom() - move.GetTo()) == 16) {
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
    std::vector<Square> pawns = GetSquares(GetPieces(color, PAWN));
    Bitboard empty_squares = GetEmptySquares();
    Direction dir = (color == WHITE) ? UP : DOWN;
    Direction op_dir = (color == WHITE) ? DOWN : UP;
    Bitboard DoublePushes = (MoveSquare(en_passant_squares[color], op_dir));
    Rank reachable_rank = (color == WHITE) ? RANK_4 : RANK_5;
    for (auto cur_pawn : pawns) {
        Bitboard pin_mask = FULL_FIELD;
        if (pinned_pieces & SquareToBitboard(cur_pawn))
            pin_mask = CalcPinMask(cur_pawn);
        Bitboard cur_moves =
            MoveSquare(SquareToBitboard(cur_pawn), dir) & empty_squares & push_mask |
            MoveSquare(MoveSquare(SquareToBitboard(cur_pawn), dir) & empty_squares, dir) & empty_squares &
                reachable_rank & push_mask |
            MoveSquare(SquareToBitboard(cur_pawn), static_cast<Direction>(dir + LEFT)) & colors[!color] & capture_mask |
            MoveSquare(SquareToBitboard(cur_pawn), static_cast<Direction>(dir + RIGHT)) & colors[!color] & capture_mask;
        cur_moves &= pin_mask;

        std::vector<Square> move_squares = GetSquares(cur_moves);
        for (Square to : move_squares) {
            if (SquareToBitboard(to) & RANK_1 || SquareToBitboard(to) & RANK_8) {
                GenPromotions(cur_pawn, to);
            } else {
                moves.push_back({cur_pawn, to});
            }
        }
        Bitboard left_en_passant = MoveSquare(SquareToBitboard(cur_pawn), static_cast<Direction>(dir + LEFT)) &
                                   en_passant_squares[color] & (push_mask | MoveToFriendSide(capture_mask)) & pin_mask;
        Square king_pos = GetFirstSquare(GetPieces(color, KING));
        Color opposite_color = static_cast<Color>(!color);
        if (left_en_passant) {
            Piece buff1 = RemovePiece(cur_pawn);
            Piece buff2 = RemovePiece(GetFirstSquare(DoublePushes));
            if (CalcMoveTable(king_pos, ~GetEmptySquares(), ROOK, color) &
                (GetPieces(opposite_color, ROOK) | GetPieces(opposite_color, QUEEN))) {
                SetPiece(buff1.type, buff1.color, cur_pawn);
                SetPiece(buff2.type, buff2.color, GetFirstSquare(DoublePushes));
                continue;
            }
            SetPiece(buff1.type, buff1.color, cur_pawn);
            SetPiece(buff2.type, buff2.color, GetFirstSquare(DoublePushes));
            Square to = GetFirstSquare(left_en_passant);
            moves.push_back({cur_pawn, to, EN_PASSANT});
        }
        Bitboard right_en_passant = MoveSquare(SquareToBitboard(cur_pawn), static_cast<Direction>(dir + RIGHT)) &
                                    en_passant_squares[color] & (push_mask | MoveToFriendSide(capture_mask)) & pin_mask;
        if (right_en_passant) {
            Piece buff1 = RemovePiece(cur_pawn);
            Piece buff2 = RemovePiece(GetFirstSquare(DoublePushes));
            if (CalcMoveTable(king_pos, ~GetEmptySquares(), ROOK, color) &
                (GetPieces(opposite_color, ROOK) | GetPieces(opposite_color, QUEEN))) {
                SetPiece(buff1.type, buff1.color, cur_pawn);
                SetPiece(buff2.type, buff2.color, GetFirstSquare(DoublePushes));
                continue;
            }
            SetPiece(buff1.type, buff1.color, cur_pawn);
            SetPiece(buff2.type, buff2.color, GetFirstSquare(DoublePushes));
            Square to = GetFirstSquare(right_en_passant);
            moves.push_back({cur_pawn, to, EN_PASSANT});
        }
    }
}

void ChessBoard::GenKnightMoves(const Color color) {
    Bitboard knights = GetPieces(color, KNIGHT);
    Bitboard friendly_pieces = colors[color];

    for (Square from : GetSquares(knights)) {
        if (pinned_pieces & SquareToBitboard(from))
            continue;
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
        Bitboard pin_mask = FULL_FIELD;
        if (pinned_pieces & SquareToBitboard(sq))
            pin_mask = CalcPinMask(sq);
        std::vector<Square> bishop_moves =
            GetSquares(CalcMoveTable(sq, board, BISHOP) & ~colors[color] & (push_mask | capture_mask) & pin_mask);
        for (auto move : bishop_moves) {
            moves.push_back({sq, move});
        }
    }
}

void ChessBoard::GenRookMoves(const Color color) {
    Bitboard rooks = GetPieces(color, ROOK);
    Bitboard board = ~GetEmptySquares();
    for (auto sq : GetSquares(rooks)) {
        Bitboard pin_mask = FULL_FIELD;
        if (pinned_pieces & SquareToBitboard(sq))
            pin_mask = CalcPinMask(sq);
        std::vector<Square> rook_moves =
            GetSquares(CalcMoveTable(sq, board, ROOK) & ~colors[color] & (push_mask | capture_mask) & pin_mask);
        for (auto move : rook_moves) {
            moves.push_back({sq, move});
        }
    }
}

void ChessBoard::GenQueenMoves(const Color color) {
    Bitboard queens = GetPieces(color, QUEEN);
    Bitboard board = ~GetEmptySquares();
    for (auto sq : GetSquares(queens)) {
        Bitboard pin_mask = FULL_FIELD;
        if (pinned_pieces & SquareToBitboard(sq))
            pin_mask = CalcPinMask(sq);
        std::vector<Square> quen_moves =
            GetSquares(CalcMoveTable(sq, board, QUEEN) & ~colors[color] & (push_mask | capture_mask) & pin_mask);
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
    if (castling_availables[color][0] && (castling_masks_check[color][0] & attack_map[!color]) == 0 &&
        (~GetEmptySquares() & castling_masks_empty[color][0]) == 0)
        moves.push_back({sq, color == WHITE ? C1 : C8, CASTLING});
    if (castling_availables[color][1] && (castling_masks_check[color][1] & attack_map[!color]) == 0 &&
        (~GetEmptySquares() & castling_masks_empty[color][1]) == 0)
        moves.push_back({sq, color == WHITE ? G1 : G8, CASTLING});
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

Bitboard ChessBoard::CalcPinMask(Square pinned_piece) {
    Color color = PieceOnSquare(pinned_piece).color;
    Color opposite_color = static_cast<Color>(!color);
    Square king_pos = GetFirstSquare(GetPieces(color, KING));
    if (global::rook_masks[king_pos] & SquareToBitboard(pinned_piece)) {
        return (CalcMoveTable(pinned_piece, ~GetEmptySquares(), ROOK, color) & global::rook_masks[king_pos]);
    }
    return (CalcMoveTable(pinned_piece, ~GetEmptySquares(), BISHOP, color) & global::bishop_masks[king_pos]);
}

void ChessBoard::CalcAttackMap(Color color) {
    Color opposite_color = static_cast<Color>(!color);
    Square king_pos = GetFirstSquare(GetPieces(opposite_color, KING));
    if (king_pos != SQUARE_NB)
        RemovePiece(king_pos);

    Bitboard board = ~GetEmptySquares();

    attack_map[color] = 0;
    for (PieceType pt : {PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING}) {
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

    Bitboard pawns_available_moves =
        (SquareToBitboard(MoveSquare(king_pos, (dir + LEFT))) | SquareToBitboard(MoveSquare(king_pos, (dir + RIGHT))));
    Bitboard bishop_available_moves = CalcMoveTable(king_pos, board, BISHOP);
    Bitboard rook_available_moves = CalcMoveTable(king_pos, board, ROOK);
    Bitboard knight_available_moves = knight_masks[king_pos];

    capture_mask = enemy_pawns & pawns_available_moves | enemy_bishops & bishop_available_moves |
                   enemy_knights & knight_available_moves | enemy_queens & bishop_available_moves |
                   enemy_queens & rook_available_moves | enemy_rooks & rook_available_moves;
    if (capture_mask == 0)
        capture_mask = FULL_FIELD;
    std::vector<Square> attacked_by_squares = GetSquares(capture_mask);
    if (attacked_by_squares.size() == 2) {
        capture_mask = 0;
    }
}

void ChessBoard::CalcPinnedPieeces(Color color) {
    pinned_pieces = 0;
    Square king_pos = GetFirstSquare(GetPieces(color, KING));
    Bitboard board = ~GetEmptySquares();
    Color opposite_color = static_cast<Color>(!color);
    std::vector<Square> enemy_sliders = GetSquares(
        (GetPieces(opposite_color, BISHOP) | GetPieces(opposite_color, QUEEN)) & global::bishop_masks[king_pos] |
        (GetPieces(opposite_color, ROOK) | GetPieces(opposite_color, QUEEN)) & global::rook_masks[king_pos]);

    for (Square slider_square : enemy_sliders) {
        PieceType cur_pt = PieceOnSquare(slider_square).type;
        if (cur_pt == BISHOP || cur_pt == ROOK) {
            pinned_pieces |= (CalcMoveTable(slider_square, board, cur_pt) & CalcMoveTable(king_pos, board, cur_pt));
        } else if (cur_pt == QUEEN) {
            if (global::rook_masks[king_pos] & SquareToBitboard(slider_square))
                pinned_pieces |= CalcMoveTable(slider_square, board, ROOK) & CalcMoveTable(king_pos, board, ROOK);
            else
                pinned_pieces |= CalcMoveTable(slider_square, board, BISHOP) & CalcMoveTable(king_pos, board, BISHOP);
        }
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
    CalcPinnedPieeces(color);
    CalcAttackMap(opposite_color);
    CalcCaptureMask(color);
    CalcPushMask(color);
}
