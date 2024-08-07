#include "move.h"

void Move::SetFrom(const Square from) {
    description &= ~FROM_MASK;
    description |= (from << FROM_BITS);
}

void Move::SetTo(const Square to) {
    description &= ~TO_MASK;
    description |= (to << TO_BITS);
}

void Move::SetType(MoveType type, PieceType promo_piece) {
    description &= ~TYPE_MASK;
    description &= (type << TYPE_BITS);
    description |= ((promo_piece - PIECE_TYPE_DELTA) << PROMO_PIECE_BITS);
}

void Move::SetType(MoveType type) {
    description &= ~TYPE_MASK;
    description |= (type << TYPE_BITS);
}

MoveType Move::GetType() const {
    return static_cast<MoveType>(description >> TYPE_BITS);
}
PieceType Move::GetPromoPieceType() const {
    return static_cast<PieceType>((description >> PROMO_PIECE_BITS) + PROMO_PIECE_BITS);
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

Move::Move(Square from, Square to, MoveType type) {
    SetFrom(from);
    SetTo(to);
    SetType(type);
}

Move::Move(Square from, Square to, MoveType type, PieceType promo_piece) {
    SetFrom(from);
    SetTo(to);
    SetType(type, promo_piece);
}

Move Move::Inversed() const {
    return Move(GetTo(), GetFrom());
}
