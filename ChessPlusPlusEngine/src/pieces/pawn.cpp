#include "pch.h"

#include "pawn.h"

////////////////////////////////////////////////////////////////////
/// Pawn
///=================================================================
Pawn::Pawn(Player team) 
    : Piece(team, PieceType::PAWN)
{
}

///=================================================================
int Pawn::getValue() const
{
    return 1;
}

///=================================================================
MoveInfo Pawn::getMoveInfo(const Move& move) const
{
    MoveInfo result;
    result.type = MoveType::IMPOSSIBLE;

    if (!move.isValid())
        return result;

    const Square& o = move.getOrigin();
    const Square& d = move.getDestination();

    int multiplier = (team == Player::WHITE ? 1 : -1);
    int startRank = (team == Player::WHITE ? 1 : 6);
    int promotionRank = (team == Player::WHITE ? 7 : 0);

    int xDiff = std::abs(o.getX() - d.getX());
    int yDiff = (d.getY() - o.getY()) * multiplier;

    if (xDiff <= 1 && yDiff == 1)
    {
        result.type = (d.getY() == promotionRank) ? MoveType::PAWN_CAPTURE_PROMOTION : MoveType::PAWN_CAPTURE;
        result.path.push_back(d);
    }

    if (xDiff == 0)
    {
        if (yDiff == 1)
        {
            result.type = (d.getY() == promotionRank) ? MoveType::PAWN_PROMOTION : MoveType::PAWN_MOVE;
            result.path.push_back(d);
        }

        if (yDiff == 2 && o.getY() == startRank)
        {
            result.type = MoveType::PAWN_DOUBLE_MOVE;
            result.path.push_back(Square(o.getX(), o.getY() + multiplier));
            result.path.push_back(d);
        }
    }

    return result;
}

///=================================================================
MoveInfoVec Pawn::getAllMovesInfo(const Square& origin) const
{
    return {};
}
