#include "pch.h"

#include "king.h"

////////////////////////////////////////////////////////////////////
/// Pawn
///=================================================================
King::King(Player team) 
    : Piece(team, PieceType::KING)
{
}

///=================================================================
int King::getValue() const
{
    return INT_MAX;
}

///=================================================================
MoveInfo King::getMoveInfo(const Move& move) const
{
    MoveInfo result;
    result.type = MoveType::IMPOSSIBLE;

    if (!move.isValid())
        return result;

    const Square& o = move.getOrigin();
    const Square& d = move.getDestination();

    if (std::abs(o.getX() - d.getX()) > 1)
        return result;

    if (std::abs(o.getY() - d.getY()) > 1)
        return result;

    result.path.push_back(d);
    result.type = MoveType::REGULAR;
    return result;
}

///=================================================================
MoveInfoVec King::getAllMovesInfo(const Square& origin) const
{
    return {};
}
