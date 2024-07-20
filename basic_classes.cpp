#include "basic_classes.h"

void ChessBoard::SetPiece(const PieceType pt, const Color c, const Square square) {
    Bitboard bb = SquareToBitboard(square);
    pieces[pt] |= bb;
    colors[c] |= bb;
}

Piece ChessBoard::RemovePiece(Square square) {
    Bitboard bb = SquareToBitboard(square);
    Piece ans = PieceOnSquare(square);
    pieces[ans.type] &= ~bb;
    colors[ans.color] &= ~bb;
    return ans;
}

bool IsOccupied(const Bitboard bb, const Square sq) {
    return ((bb & SquareToBitboard(sq)) >> sq);
}

Piece ChessBoard::PieceOnSquare(const Square sq) {
    uint8_t piece_type = 0;
    Color piece_color =
        static_cast<Color>(WHITE * IsOccupied(colors[WHITE], sq) + BLACK * IsOccupied(colors[BLACK], sq));

    for (uint8_t pt = 0; pt < PIECE_NB; ++pt)
        piece_type += pt * IsOccupied(pieces[pt], sq);
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

ChessBoard::ChessBoard() {
    colors.fill(0);
    pieces.fill(0);
    pieces[NONE] = -1;
}

ChessBoard::ChessBoard(std::string input) {
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
    std::cout << "gjhegeigh\n";
    for (int i = 7; i >= 0; --i) {
        for (int j = 0; j < 8; ++j) {
            std::cout << board[i][j] << ' ';
        }
        std::cout << '\n';
    }
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

int main() {
    ChessBoard hgh("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
    hgh.MakeMove({E1, E5});
    hgh.MakeMove({E1, E5});
    hgh.MakeMove({E5, E1});

    hgh.PrintBoard();
}
