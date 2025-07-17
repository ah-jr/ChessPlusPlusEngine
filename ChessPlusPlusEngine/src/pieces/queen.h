#ifndef QUEEN_HEADER
#define QUEEN_HEADER

#include "base.h"
#include <vector>

////////////////////////////////////////////////////////////////////
/// Queen
///=================================================================
class Queen : public Piece
{
    public:
        Queen(Team team); 
        const int getValue();
        bool checkValidMove(Square move, Board* board);
        std::vector<Square>* getValidMoves(Board* board, Square pos);
};

#endif