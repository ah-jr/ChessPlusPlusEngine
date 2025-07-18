#ifndef KNIGHT_HEADER
#define KNIGHT_HEADER

#include "../basic.h"

////////////////////////////////////////////////////////////////////
/// Knight
///=================================================================
class Knight : public Piece
{
public:
    Knight(Player team); 

    int getValue() const;
    MoveInfo getMoveInfo(const Move& move) const;
    MoveInfoVec getAllMovesInfo(const Square& origin) const;

    DECLARE_CLONABLE(Knight);
};

#endif