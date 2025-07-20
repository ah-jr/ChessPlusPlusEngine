#include "pch.h"

#include "queen.h"

////////////////////////////////////////////////////////////////////
/// Queen
///=================================================================
Queen::Queen(Team team) 
    : GenericPiece({ PieceType::QUEEN, team })
{
    moveInLines = true;
    moveInDiagonals = true;
}

///=================================================================
int Queen::getValue() const
{
    return 9;
}

///=================================================================
int Queen::getMoveRange() const
{
    return 7;
}

///=================================================================
DirectionSet Queen::getMoveDirections() const
{
    return {
        { -1, -1 },
        { -1,  0 },
        { -1,  1 },
        {  0, -1 },
        {  0,  1 },
        {  1, -1 },
        {  1,  0 },
        {  1,  1 }
    };
}

///=================================================================
DirectionSet Queen::getAttackDirections() const
{
    return getMoveDirections();
}

///=================================================================
MoveType Queen::getMoveType(const Move& move) const
{
    return getGenericMoveType(move);
}

///=================================================================
SquareVec Queen::getMovePath(const Move& move) const
{
    return getGenericMovePath(move);
}
