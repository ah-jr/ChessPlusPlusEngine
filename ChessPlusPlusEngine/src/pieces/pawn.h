#ifndef PAWN_HEADER
#define PAWN_HEADER

#include "../basic.h"

////////////////////////////////////////////////////////////////////
/// Pawn
///=================================================================
class Pawn : public Piece
{
public:
    Pawn(Player team); 

    int getValue() const;
    MoveInfo getMoveInfo(const Move& move) const;
    MoveInfoVec getAllMovesInfo(const Square& origin) const;

    DECLARE_CLONABLE(Pawn);
};

#endif