#include "pch.h"

#include "pawn.h"

////////////////////////////////////////////////////////////////////
/// Pawn
///=================================================================
Pawn::Pawn(Team team) 
    : Piece({ PieceType::PAWN, team })
{
}

///=================================================================
int Pawn::getValue() const
{
    return 1;
}

///=================================================================
int Pawn::getMoveRange() const
{
    return 1;
}

///=================================================================
DirectionSet Pawn::getMoveDirections() const
{
    if (info.team == Team::WHITE)
        return {
            {  0,  1 },
            {  0,  2 },
        };

    return {
        {  0, -1 },
        {  0, -2 },
    };
}

///=================================================================
DirectionSet Pawn::getAttackDirections() const
{
    if (info.team == Team::WHITE)
        return {
            { -1,  1 },
            {  1,  1 }
    };

    return {
        { -1, -1 },
        {  1, -1 }
    };
}

///=================================================================
MoveType Pawn::getMoveType(const Move& move) const
{
    if (!move.isValid())
        return MoveType::IMPOSSIBLE;

    int direction = (info.team == Team::WHITE ? 1 : -1);
    int startRank = (info.team == Team::WHITE ? 1 : 6);
    int promoRank = (info.team == Team::WHITE ? 7 : 0);

    const Square& o = move.getOrigin();
    const Square& d = move.getDestination();

    int xDiff = move.getDeltaX();
    int yDiff = move.getDeltaY() * direction;

    if (std::abs(xDiff) == 1 && yDiff == 1)
        return (d.getY() == promoRank) ? MoveType::PAWN_CAPTURE_PROMOTION : MoveType::PAWN_CAPTURE;

    if (std::abs(xDiff) == 0)
    {
        if (yDiff == 1)
            return (d.getY() == promoRank) ? MoveType::PAWN_PROMOTION : MoveType::PAWN_MOVE;

        if (yDiff == 2 && o.getY() == startRank)
            return MoveType::PAWN_DOUBLE_MOVE;
    }

    return MoveType::IMPOSSIBLE;
}

///=================================================================
SquareVec Pawn::getMovePath(const Move& move) const
{
    SquareVec result;

    if (getMoveType(move) == MoveType::IMPOSSIBLE)
        return result;

    result.push_back(move.getDestination());

    if (std::abs(move.getDeltaY()) == 2)
        result.emplace_back(move.getOrigin().getX(), move.getOrigin().getY() + move.getDeltaY() / 2);

    return result;
}
