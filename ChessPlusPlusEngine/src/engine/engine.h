#ifndef ENGINE_HEADER
#define ENGINE_HEADER

#include "..\basic.h"
#include "..\game.h"

////////////////////////////////////////////////////////////////////
/// Engine
///=================================================================
class Engine
{
    public:
        Move getNextMove(Game* game, Board* board, Team team, int depth);
        int  getNextMoveR(Game* game, const Move& move, Board* board, SquareVec* wPieces, SquareVec* bPieces, Team team, int depth);
};

#endif