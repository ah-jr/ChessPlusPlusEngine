#ifndef KING_HEADER
#define KING_HEADER

#include "../basic.h"

////////////////////////////////////////////////////////////////////
/// King
///=================================================================
class King : public Piece
{
public:
    King(Team team); 

    int getValue() const;
    int getMoveRange() const;
    DirectionSet getMoveDirections() const;
    DirectionSet getAttackDirections() const;
    MoveType getMoveType(const Move& move) const;
    SquareVec getMovePath(const Move& move) const;

    DECLARE_CLONABLE(King);
};

#endif