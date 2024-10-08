#include "magic.h"
#include "basic_classes.h"

enum MoveType : uint16_t { NORMAL = 0, PROMOTION = 1, EN_PASSANT = 2, CASTLING = 3 };

class Move {
public:
    Move() = default;
    Move(Square from, Square to);
    Move(Square from, Square to, MoveType type);
    Move(Square from, Square to, MoveType type, PieceType promo_piece);

    void SetFrom(Square from);
    void SetTo(Square to);
    void SetType(MoveType type, PieceType promo_piece);
    void SetType(MoveType type);

    MoveType GetType() const;
    PieceType GetPromoPieceType() const;
    std::string GetUCImove() const;
    Square GetFrom() const;
    Square GetTo() const;

    Move Inversed() const;

    Move SuckCock();

private:
    uint16_t description = 0;
    /*
    0-5  -- initial square
    6-11 -- destination square
    0 << 12 - normal
    1 << 12 - castling
    2 << 12 - promotion
    3 << 12 - en_passant
    14-15 - destination piece
    */
};