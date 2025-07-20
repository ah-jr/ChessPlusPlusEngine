#ifndef KNIGHT_HEADER
#define KNIGHT_HEADER

#include "../basic.h"

////////////////////////////////////////////////////////////////////
/// Knight
///=================================================================
class Knight : public Piece
{
public:
    Knight(Team team); 

    int getValue() const;
    int getMoveRange() const;
    DirectionSet getMoveDirections() const;
    DirectionSet getAttackDirections() const;
    MoveType getMoveType(const Move& move) const;
    SquareVec getMovePath(const Move& move) const;

    DECLARE_CLONABLE(Knight);
};

#endif