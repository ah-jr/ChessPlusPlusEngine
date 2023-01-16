#ifndef KNIGHT_HEADER
#define KNIGHT_HEADER

#include "base.h"
#include <vector>

////////////////////////////////////////////////////////////////////
/// Knight
///=================================================================
class Knight : public Piece
{
    public:
        Knight(Team team); 
        const int getValue();
        bool checkValidMove(Square move, Board* board);
        std::vector<Square>* getValidMoves(Board* board, Square pos);
};

#endif