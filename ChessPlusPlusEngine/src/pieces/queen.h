#ifndef QUEEN_HEADER
#define QUEEN_HEADER

#include "../basic.h"

////////////////////////////////////////////////////////////////////
/// Queen
///=================================================================
class Queen : public Piece
{
public:
    Queen(Player team); 

    int getValue() const;
    MoveInfo getMoveInfo(const Move& move) const;
    MoveInfoVec getAllMovesInfo(const Square& origin) const;

    DECLARE_CLONABLE(Queen);
};

#endif