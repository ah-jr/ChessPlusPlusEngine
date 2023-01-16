#ifndef KING_HEADER
#define KING_HEADER

#include "base.h"
#include <vector>

////////////////////////////////////////////////////////////////////
/// King
///=================================================================
class King : public Piece
{
    public:
        King(Team team); 
        const int getValue();
        bool checkValidMove(Square move, Board* board);
        std::vector<Square>* getValidMoves(Board* board, Square pos);
};

#endif