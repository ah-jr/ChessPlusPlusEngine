#ifndef BISHOP_HEADER
#define BISHOP_HEADER

#include "../basic.h"

////////////////////////////////////////////////////////////////////
/// Bishop
///=================================================================
class Bishop : public Piece
{
public:
    Bishop(Player team); 

    int getValue() const;
    MoveInfo getMoveInfo(const Move& move) const;
    MoveInfoVec getAllMovesInfo(const Square& origin) const;

    DECLARE_CLONABLE(Bishop);
};

#endif