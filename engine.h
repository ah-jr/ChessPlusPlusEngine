#ifndef ENGINE_HEADER
#define ENGINE_HEADER

#include "base.h"

////////////////////////////////////////////////////////////////////
/// Engine
///=================================================================
class Engine
{
    private:
        Board* m_board;

    public:
        Engine(Board* board); 

        Square getNextMove(Team team, int depth);
};

#endif