#ifndef BISHOP_HEADER
#define BISHOP_HEADER

#include "base.h"

////////////////////////////////////////////////////////////////////
/// Bishop
///=================================================================
class Bishop : public Piece
{
    public:
        Bishop(Team team); 
        bool checkValidMove(Square move, Board* board);
};

#endif