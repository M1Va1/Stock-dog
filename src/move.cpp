#include "move.h"

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

