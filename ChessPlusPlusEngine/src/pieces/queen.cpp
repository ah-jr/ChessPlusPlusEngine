#include "pch.h"

#include "queen.h"

////////////////////////////////////////////////////////////////////
/// Queen
///=================================================================
Queen::Queen(Player team) 
    : Piece(team, PieceType::QUEEN)
{
}

///=================================================================
int Queen::getValue() const
{
    return 9;
}

///=================================================================
MoveInfo Queen::getMoveInfo(const Move& move) const
{
    MoveInfo result;
    result.type = MoveType::IMPOSSIBLE;

    if (!move.isValid())
        return result;

    const Square& o = move.getOrigin();
    const Square& d = move.getDestination();

    int xDiff = std::abs(o.getX() - d.getX());
    int yDiff = std::abs(o.getY() - d.getY());

    if (xDiff != yDiff && xDiff * yDiff)
        return result;

    if (xDiff == yDiff) 
    {
        auto xVals = getValuesBetween(o.getX(), d.getX());
        auto yVals = getValuesBetween(o.getY(), d.getY());

        for (int i = 0; i < xDiff; i++)
            result.path.push_back(Square(xVals[i], yVals[i]));
    }

    if (xDiff == 0)
        for (int n : getValuesBetween(o.getY(), d.getY()))
            result.path.push_back(Square(o.getX(), n));

    if (yDiff == 0)
        for (int n : getValuesBetween(o.getX(), d.getX()))
            result.path.push_back(Square(n, o.getY()));

    result.type = MoveType::REGULAR;
    return result;
}

///=================================================================
MoveInfoVec Queen::getAllMovesInfo(const Square& origin) const
{
    return {};
}
