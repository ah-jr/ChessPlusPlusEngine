#ifndef PAWN_HEADER
#define PAWN_HEADER

#include "../basic.h"

////////////////////////////////////////////////////////////////////
/// Pawn
///=================================================================
class Pawn : public Piece
{
public:
    Pawn(Team team); 

    int getValue() const;
    int getMoveRange() const;
    DirectionSet getMoveDirections() const;
    DirectionSet getAttackDirections() const;
    MoveType getMoveType(const Move& move) const;
    SquareVec getMovePath(const Move& move) const;

    DECLARE_CLONABLE(Pawn);
};

#endif