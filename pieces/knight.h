#ifndef KNIGHT_HEADER
#define KNIGHT_HEADER

#include "base.h"

////////////////////////////////////////////////////////////////////
/// Knight
///=================================================================
class Knight : public Piece
{
    public:
        Knight(Team team); 
        bool checkValidMove(Square move, Board* board);
};

#endif