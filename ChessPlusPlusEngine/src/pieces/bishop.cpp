#include "pch.h"

#include "bishop.h"

////////////////////////////////////////////////////////////////////
/// Bishop
///=================================================================
Bishop::Bishop(Player team) 
    : Piece(team, PieceType::BISHOP)
{
}

///=================================================================
int Bishop::getValue() const
{
    return 3;
}

///=================================================================
MoveInfo Bishop::getMoveInfo(const Move& move) const
{
    MoveInfo result;
    result.type = MoveType::IMPOSSIBLE;

    if (!move.isValid())
        return result;

    const Square& o = move.getOrigin();
    const Square& d = move.getDestination();

    int xDiff = std::abs(o.getX() - d.getX());
    int yDiff = std::abs(o.getY() - d.getY());

    if (xDiff != yDiff)
        return result;

    auto xVals = getValuesBetween(o.getX(), d.getX());
    auto yVals = getValuesBetween(o.getY(), d.getY());

    for (int i = 0; i < xDiff; i++)
        result.path.push_back(Square(xVals[i], yVals[i]));

    result.type = MoveType::REGULAR;
    return result;
}

///=================================================================
MoveInfoVec Bishop::getAllMovesInfo(const Square& origin) const
{
    return {};
}
