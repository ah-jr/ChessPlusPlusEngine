#ifndef PAWN_HEADER
#define PAWN_HEADER

#include "base.h"
#include <vector>

////////////////////////////////////////////////////////////////////
/// Pawn
///=================================================================
class Pawn : public Piece
{
    public:
        Pawn(Team team); 
        const int getValue();
        bool checkValidMove(Square move, Board* board);
        std::vector<Square>* getValidMoves(Board* board);
};

#endif