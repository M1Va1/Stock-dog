#include "basic_classes.h"

void ChessBoard::setPiece(PieceType pt, Color c, Square square) {
    Bitboard bb = squareToBitboard(square);
    pieces[pt] |= bb;
    colors[c] |= bb;
}

void ChessBoard::removePiece(PieceType pt, Color c, Square square) {
    Bitboard bb = squareToBitboard(square);
    pieces[pt] &= ~bb;
    colors[c] &= ~bb;
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
