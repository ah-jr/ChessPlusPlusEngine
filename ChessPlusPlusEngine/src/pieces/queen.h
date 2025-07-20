#ifndef QUEEN_HEADER
#define QUEEN_HEADER

#include "../basic.h"

////////////////////////////////////////////////////////////////////
/// Queen
///=================================================================
class Queen : public GenericPiece
{
public:
    Queen(Team team); 

    int getValue() const;
    int getMoveRange() const;
    DirectionSet getMoveDirections() const;
    DirectionSet getAttackDirections() const;
    MoveType getMoveType(const Move& move) const;
    SquareVec getMovePath(const Move& move) const;

    DECLARE_CLONABLE(Queen);
};

#endif