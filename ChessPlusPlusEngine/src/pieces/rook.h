#ifndef ROOK_HEADER
#define ROOK_HEADER

#include "../basic.h"

////////////////////////////////////////////////////////////////////
/// Rook
///=================================================================
class Rook : public Piece
{
public:
    Rook(Player team); 

    int getValue() const;
    MoveInfo getMoveInfo(const Move& move) const;
    MoveInfoVec getAllMovesInfo(const Square& origin) const;

    DECLARE_CLONABLE(Rook);
};

#endif