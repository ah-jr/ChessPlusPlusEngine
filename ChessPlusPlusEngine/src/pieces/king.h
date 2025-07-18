#ifndef KING_HEADER
#define KING_HEADER

#include "../basic.h"

////////////////////////////////////////////////////////////////////
/// King
///=================================================================
class King : public Piece
{
public:
    King(Player team); 

    int getValue() const;
    MoveInfo getMoveInfo(const Move& move) const;
    MoveInfoVec getAllMovesInfo(const Square& origin) const;

    DECLARE_CLONABLE(King);
};

#endif