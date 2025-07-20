#include "pch.h"

#include "rook.h"

////////////////////////////////////////////////////////////////////
/// Rook
///=================================================================
Rook::Rook(Team team) 
    : GenericPiece({ PieceType::ROOK, team })
{
    moveInLines = true;
}

///=================================================================
int Rook::getValue() const
{
    return 5;
}

///=================================================================
int Rook::getMoveRange() const
{
    return 7;
}

///=================================================================
DirectionSet Rook::getMoveDirections() const
{
    return {
        { -1,  0 },
        {  0, -1 },
        {  0,  1 },
        {  1,  0 },
    };
}

///=================================================================
DirectionSet Rook::getAttackDirections() const
{
    return getMoveDirections();
}

///=================================================================
MoveType Rook::getMoveType(const Move& move) const
{
    return getGenericMoveType(move);
}

///=================================================================
SquareVec Rook::getMovePath(const Move& move) const
{
    return getGenericMovePath(move);
}
