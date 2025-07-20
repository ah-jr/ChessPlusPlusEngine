#ifndef BISHOP_HEADER
#define BISHOP_HEADER

#include "../basic.h"

////////////////////////////////////////////////////////////////////
/// Bishop
///=================================================================
class Bishop : public GenericPiece
{
public:
    Bishop(Team team); 

    int getValue() const;
    int getMoveRange() const;
    DirectionSet getMoveDirections() const;
    DirectionSet getAttackDirections() const;
    MoveType getMoveType(const Move& move) const;
    SquareVec getMovePath(const Move& move) const;

    DECLARE_CLONABLE(Bishop);
};

#endif