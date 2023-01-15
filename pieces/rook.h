#ifndef ROOK_HEADER
#define ROOK_HEADER

#include "base.h"
#include <vector>

////////////////////////////////////////////////////////////////////
/// Rook
///=================================================================
class Rook : public Piece
{
    public:
        Rook(Team team); 
        const int getValue();
        bool checkValidMove(Square move, Board* board);
        std::vector<Square>* getValidMoves(Board* board);
};

#endif