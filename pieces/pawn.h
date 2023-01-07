#ifndef PAWN_HEADER
#define PAWN_HEADER

#include "base.h"

////////////////////////////////////////////////////////////////////
/// Pawn
///=================================================================
class Pawn : public Piece
{
    public:
        Pawn(Team team); 
        bool checkValidMove(Square move, Board* board);
};

#endif