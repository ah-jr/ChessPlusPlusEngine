#ifndef ROOK_HEADER
#define ROOK_HEADER

#include "../basic.h"

////////////////////////////////////////////////////////////////////
/// Rook
///=================================================================
class Rook : public GenericPiece
{
public:
    Rook(Team team); 

    int getValue() const;
    int getMoveRange() const;
    DirectionSet getMoveDirections() const;
    DirectionSet getAttackDirections() const;
    MoveType getMoveType(const Move& move) const;
    SquareVec getMovePath(const Move& move) const;

    DECLARE_CLONABLE(Rook);
};

#endif