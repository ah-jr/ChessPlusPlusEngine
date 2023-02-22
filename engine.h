#ifndef ENGINE_HEADER
#define ENGINE_HEADER

#include "pieces\base.h"

////////////////////////////////////////////////////////////////////
/// Engine
///=================================================================
class Engine
{
    private:
        Board* m_board;

    public:
        Engine(Board* board); 

        Move getNextMove(Board* board, Team team, int depth);
        int  getNextMoveR(const Move& move, Board* board, SquareVec wPieces, SquareVec bPieces, Team team, int depth);
};

#endif