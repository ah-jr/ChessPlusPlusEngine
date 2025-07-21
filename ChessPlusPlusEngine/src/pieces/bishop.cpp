#include "pch.h"

#include "bishop.h"

////////////////////////////////////////////////////////////////////
/// Bishop
///=================================================================
Bishop::Bishop(Team team) 
    : GenericPiece({ PieceType::BISHOP, team })
{
    moveInDiagonals = true;
}

///=================================================================
int Bishop::getValue() const
{
    return 3;
}

///=================================================================
int Bishop::getMoveRange() const
{
    return 7;
}

///=================================================================
DirectionSet Bishop::getMoveDirections() const
{
    return {};
}

///=================================================================
DirectionSet Bishop::getAttackDirections() const
{
    return {
        { -1, -1 },
        { -1,  1 },
        {  1, -1 },
        {  1,  1 }
    };
}

///=================================================================
MoveType Bishop::getMoveType(const Move& move) const
{
    return getGenericMoveType(move);
}

///=================================================================
SquareVec Bishop::getMovePath(const Move& move) const
{
    return getGenericMovePath(move);
}
