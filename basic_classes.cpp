#include "basic_classes.h"

void ChessBoard::setPiece(const PieceType& pt, const Color& c, const Square& square) {
    Bitboard bb = squareToBitboard(square);
    pieces[pt] |= bb;
    colors[c] |= bb;
}

bool IsTrueInSqure(const Bitboard& bb, const Square& sq) {
    return ((bb & squareToBitboard(sq)) >> sq);
}

// bool ChessBoard::AvailableMove(Move& move) {
//     Piece active_piece = PieceOnSquare(move.from);
//     if (active_piece.type == NONE)
//         return 0;
//     if (active_piece.type == PAWN) {
//     }
// }

Piece ChessBoard::PieceOnSquare(Square& sq) {
    uint8_t AnsPt = 0;
    Color AnsColor = static_cast<Color>(uint8_t(WHITE) * IsTrueInSqure(colors[WHITE], sq) +
                                        uint8_t(BLACK) * IsTrueInSqure(colors[BLACK], sq));
    for (uint8_t i = 0; i < PIECE_NB; ++i)
        AnsPt += i * IsTrueInSqure(pieces[i], sq);
    return {static_cast<PieceType>(AnsPt), AnsColor};
};

Piece ChessBoard::removePiece(Square& square) {
    Bitboard bb = squareToBitboard(square);
    Piece ans = PieceOnSquare(square);
    pieces[ans.type] &= ~bb;
    colors[ans.color] &= ~bb;
    return ans;
}

void ChessBoard::DoMove(Move cur_move) {
    Piece buff = removePiece(cur_move.from);
    removePiece(cur_move.to);
    setPiece(buff.type, buff.color, cur_move.to);
}

ChessBoard FenEncoder(std::string input) {
    ChessBoard NewChessboard;
    size_t cur_ind = 0;
    for (int rank = 7; rank >= 0; --rank) {
        int file = 0;
        while (file < 8) {
            if (isdigit(input[cur_ind])) {
                file += (input[cur_ind] - '0');
                ++cur_ind;
            } else {
                NewChessboard.setPiece(FenPiecesCodes[tolower(input[cur_ind])], (Color)islower(input[cur_ind]),
                                       WhichSquare(rank, file));
                ++cur_ind;
                ++file;
            }
        }
        ++cur_ind;
    }
    return NewChessboard;
}

ChessBoard::ChessBoard() {
    colors[0] = 0;
    colors[1] = 0;
    for (int i = 1; i < PIECE_NB; ++i) {
        pieces[i] = 0;
    }
    pieces[NONE] = -1;
}

ChessBoard ChessBoard::operator=(const ChessBoard& rhs) {
    ChessBoard ans;
    ans.colors = rhs.colors;
    ans.pieces = rhs.pieces;
    return ans;
}

ChessBoard::ChessBoard(std::string input) {
    ChessBoard buff = FenEncoder(input);
    colors = buff.colors;
    pieces = buff.pieces;
}

void ChessBoard::printBoard() {
    char board[8][8];
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            board[i][j] = '.';
        }
    }

    for (int pt = 0; pt < PIECE_NB; ++pt) {
        for (int sq = 0; sq < 64; ++sq) {
            if (pieces[pt] & squareToBitboard(sq)) {
                int row = sq / 8;
                int col = sq % 8;
                char pieceChar;
                switch (pt) {
                    case NONE:
                        pieceChar = '.';
                        break;
                    case PAWN:
                        pieceChar = 'P';
                        break;
                    case KNIGHT:
                        pieceChar = 'N';
                        break;
                    case BISHOP:
                        pieceChar = 'B';
                        break;
                    case ROOK:
                        pieceChar = 'R';
                        break;
                    case QUEEN:
                        pieceChar = 'Q';
                        break;
                    case KING:
                        pieceChar = 'K';
                        break;
                }
                if (colors[BLACK] & squareToBitboard(sq)) {
                    pieceChar = tolower(pieceChar);
                }
                board[row][col] = pieceChar;
            }
        }
    }

    for (int i = 7; i >= 0; --i) {
        for (int j = 0; j < 8; ++j) {
            std::cout << board[i][j] << ' ';
        }
        std::cout << '\n';
    }
}

int main() {
    ChessBoard hgh("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
    // ChessBoard hgh;
    hgh.DoMove({E1, A8});
    hgh.DoMove({E7, E5});
    hgh.printBoard();
    // std::cout << hgh.colors[0] << '\n' << hgh.pieces[0];
}
