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
