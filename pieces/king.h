#ifndef KING_HEADER
#define KING_HEADER

#include "base.h"

////////////////////////////////////////////////////////////////////
/// King
///=================================================================
class King : public Piece
{
    public:
        King(Team team); 
        bool checkValidMove(Square move, Board* board);
};

#endif