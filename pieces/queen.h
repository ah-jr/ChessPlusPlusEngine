#ifndef QUEEN_HEADER
#define QUEEN_HEADER

#include "base.h"

////////////////////////////////////////////////////////////////////
/// Queen
///=================================================================
class Queen : public Piece
{
    public:
        Queen(Team team); 
        bool checkValidMove(Square move, Board* board);
};

#endif