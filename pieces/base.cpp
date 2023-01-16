#include "base.h"

////////////////////////////////////////////////////////////////////
/// Piece
///=================================================================
Piece::Piece(Team team, PieceType type)
{
    this->team = team;
    this->type = type;
}

///=================================================================
PieceType Piece::getType()
{
    return this->type;
}

///=================================================================
bool Piece::checkValidSquare(Square square)
{
    return square.x >= 0 && square.x <= 7 && square.y >=0 && square.y <= 7;
}

////////////////////////////////////////////////////////////////////
/// Team
///=================================================================
Team GetOppositeTeam(Team team)
{
    return team == Team::WHITE ? Team::BLACK : Team::WHITE; 
}
