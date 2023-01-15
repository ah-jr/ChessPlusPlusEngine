#ifndef BISHOP_HEADER
#define BISHOP_HEADER

#include "base.h"
#include <vector>

////////////////////////////////////////////////////////////////////
/// Bishop
///=================================================================
class Bishop : public Piece
{
    public:
        Bishop(Team team); 
        const int getValue();
        bool checkValidMove(Square move, Board* board);
        std::vector<Square>* getValidMoves(Board* board);
};

#endif