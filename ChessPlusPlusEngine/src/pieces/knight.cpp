#include "pch.h"

#include "knight.h"

////////////////////////////////////////////////////////////////////
/// Knight
///=================================================================
Knight::Knight(Player team) 
    : Piece(team, PieceType::KNIGHT)
{
}

///=================================================================
int Knight::getValue() const
{
    return 3;
}

///=================================================================
MoveInfo Knight::getMoveInfo(const Move& move) const
{
    MoveInfo result;
    result.type = MoveType::IMPOSSIBLE;

    if (!move.isValid())
        return result;

    const Square& o = move.getOrigin();
    const Square& d = move.getDestination();

    int xDiff = std::abs(o.getX() - d.getX());
    int yDiff = std::abs(o.getY() - d.getY());

    if (xDiff * yDiff == 2)
    {
        result.type = MoveType::REGULAR;
        result.path.push_back(d);
    }
    
    return result;
}

///=================================================================
MoveInfoVec Knight::getAllMovesInfo(const Square& origin) const
{
    return {};
}
