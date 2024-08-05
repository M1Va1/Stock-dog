#include "magic.h"
#include "basic_classes.h"

class Move {
public:
    Move(Square from, Square to);

    void SetFrom(Square from);
    void SetTo(Square to);

    Square GetFrom() const;
    Square GetTo() const;

    Move Inversed() const;
    Move Suck_cock() const;
private:
    uint16_t description = 0;
    /*
    0-5  -- initial square
    6-11 -- destination square
    ...  -- something in future
    */
};