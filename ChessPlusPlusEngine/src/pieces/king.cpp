#include "pch.h"

#include "king.h"

////////////////////////////////////////////////////////////////////
/// Pawn
///=================================================================
King::King(Team team) 
    : Piece({ PieceType::KING, team })
{
}

///=================================================================
int King::getValue() const
{
    return 1000;
}

///=================================================================
int King::getMoveRange() const
{
    return 1;
}

///=================================================================
DirectionSet King::getMoveDirections() const
{
    return {};
}

///=================================================================
DirectionSet King::getAttackDirections() const
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
MoveType King::getMoveType(const Move& move) const
{
    if (!move.isValid())
        return MoveType::IMPOSSIBLE;

    if (std::abs(move.getDeltaX()) > 1)
        return MoveType::IMPOSSIBLE;

    if (std::abs(move.getDeltaY()) > 1)
        return MoveType::IMPOSSIBLE;

    return MoveType::REGULAR;
}

///=================================================================
SquareVec King::getMovePath(const Move& move) const
{
    SquareVec result;

    if (getMoveType(move) == MoveType::IMPOSSIBLE)
        return result;

    result.push_back(move.getDestination());
    return result;
}

