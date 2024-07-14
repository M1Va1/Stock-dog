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
    for (int i = 0; i < PIECE_NB; ++i) {
        pieces[i] = 0;
    }
}

ChessBoard ChessBoard::operator=(const ChessBoard& rhs) {
    ChessBoard ans;
    memcpy(ans.colors, rhs.colors, sizeof rhs.colors);
    memcpy(ans.pieces, rhs.pieces, sizeof rhs.pieces);
    return ans;
}

ChessBoard::ChessBoard(std::string input) {
    ChessBoard buff = FenEncoder(input);
    memcpy(colors, buff.colors, sizeof buff.colors);
    memcpy(pieces, buff.pieces, sizeof buff.pieces);
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

int main() {
    ChessBoard hgh("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
    // ChessBoard hgh;
    hgh.printBoard();
    // std::cout << hgh.colors[0] << '\n' << hgh.pieces[0];
}
