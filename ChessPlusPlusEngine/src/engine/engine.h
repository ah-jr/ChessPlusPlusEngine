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
    Move getNextMove(Game* game);
    void setDepth(int depth);

private:
    float getPieceEvaluation(const Square& position, const Piece* piece);
    float getNextMoveR(Game* game, int step, float alpha, float beta);

    Move bestMove;
    int depth;
    std::vector<Square> whitePieceLocations;
    std::vector<Square> blackPieceLocations;

    float currentEvaluation;

    std::unordered_map<std::string, float> cache;
    std::string boardString;
    Team player;
    int totalCalls;
    int skippedCalls;
    int evaluations;
};

#endif