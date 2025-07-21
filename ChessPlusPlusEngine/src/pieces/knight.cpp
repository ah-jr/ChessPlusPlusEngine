#include "pch.h"

#include "knight.h"

////////////////////////////////////////////////////////////////////
/// Knight
///=================================================================
Knight::Knight(Team team) 
    : Piece({ PieceType::KNIGHT, team })
{
}

///=================================================================
int Knight::getValue() const
{
    return 3;
}

///=================================================================
int Knight::getMoveRange() const
{
    return 1;
}

///=================================================================
DirectionSet Knight::getMoveDirections() const
{
    return {};
}

///=================================================================
DirectionSet Knight::getAttackDirections() const
{
    return {
    { -2, -1 },
    { -2,  1 },
    { -1, -2 },
    { -1,  2 },
    {  1, -2 },
    {  1,  2 },
    {  2, -1 },
    {  2,  1 }
    };
}

///=================================================================
MoveType Knight::getMoveType(const Move& move) const
{
    if (!move.isValid())
        return MoveType::IMPOSSIBLE;

    if (abs(move.getDeltaX() * move.getDeltaY()) == 2)
        return MoveType::REGULAR;

    return MoveType::IMPOSSIBLE;
}

///=================================================================
SquareVec Knight::getMovePath(const Move& move) const
{
    SquareVec result;

    if (getMoveType(move) == MoveType::IMPOSSIBLE)
        return result;

    result.push_back(move.getDestination());
    return result;
}
