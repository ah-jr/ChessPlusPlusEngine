#include "pch.h"

#include "rook.h"

////////////////////////////////////////////////////////////////////
/// Rook
///=================================================================
Rook::Rook(Player team) 
    : Piece(team, PieceType::ROOK)
{
}

///=================================================================
int Rook::getValue() const
{
    return 5;
}

///=================================================================
MoveInfo Rook::getMoveInfo(const Move& move) const
{
    MoveInfo result;
    result.type = MoveType::IMPOSSIBLE;

    if (!move.isValid())
        return result;

    const Square& o = move.getOrigin();
    const Square& d = move.getDestination();

    int xDiff = std::abs(o.getX() - d.getX());
    int yDiff = std::abs(o.getY() - d.getY());

    if (xDiff * yDiff)
        return result;

    if (xDiff == 0)
    {
        int sign = o.getY() < d.getY() ? 1 : -1;
        for (int i = 1; i <= yDiff; i++)
            result.path.push_back(Square(o.getX(), o.getY() + i * sign));
    }

    if (yDiff == 0)
    {
        int sign = o.getX() < d.getX() ? 1 : -1;
        for (int i = 1; i <= xDiff; i++)
            result.path.push_back(Square(o.getX() + i * sign, o.getY()));
    }

    result.type = MoveType::REGULAR;
    return result;
}

///=================================================================
MoveInfoVec Rook::getAllMovesInfo(const Square& origin) const
{
    return {};
}
