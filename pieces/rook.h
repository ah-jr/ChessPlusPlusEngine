#ifndef ROOK_HEADER
#define ROOK_HEADER

#include "base.h"

////////////////////////////////////////////////////////////////////
/// Rook
///=================================================================
class Rook : public Piece
{
    public:
        Rook(Team team); 
        bool checkValidMove(Square move, Board* board);
};

#endif