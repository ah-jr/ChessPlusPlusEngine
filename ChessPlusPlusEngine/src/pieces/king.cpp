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
DirectionSet King::getAttackDirections() const
{
    return getMoveDirections();
}

///=================================================================
MoveType King::getMoveType(const Move& move) const
{
    if (!move.isValid())
        return MoveType::IMPOSSIBLE;

    if (std::abs(move.getDeltaX()) > 1)
    {
        if (std::abs(move.getDeltaY()) != 0)
            return MoveType::IMPOSSIBLE;

        int dir = (info.team == Team::WHITE) ? 1 : -1;
        if (move.getDeltaX() == 2 * dir)
            return MoveType::SHORT_CASTLE;
        if (move.getDeltaX() == -2 * dir)
            return MoveType::LONG_CASTLE;

        return MoveType::IMPOSSIBLE;
    } 

    if (std::abs(move.getDeltaY()) > 1)
        return MoveType::IMPOSSIBLE;

    return MoveType::REGULAR;
}

///=================================================================
SquareVec King::getMovePath(const Move& move) const
{
    SquareVec result;
    const MoveType& type = getMoveType(move);

    if (type == MoveType::IMPOSSIBLE)
        return result;

    int dir = (info.team == Team::WHITE) ? 1 : -1;
    if (type == MoveType::SHORT_CASTLE)
        result.emplace_back(move.getOrigin().getX() + dir, move.getOrigin().getY());

    if (type == MoveType::LONG_CASTLE)
        result.emplace_back(move.getOrigin().getX() - dir, move.getOrigin().getY());

    result.push_back(move.getDestination());
    return result;
}

