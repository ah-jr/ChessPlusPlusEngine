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
    Move getNextMove(Game* referenceGame);
    void setDepth(int depth);

private:
    MoveVec getValidMoves(const Square& origin);
    float getPieceEvaluation(const Square& position, const Piece* piece);
    float getNextMoveR(int step, float alpha, float beta, bool maximize);

    Board board;
    Team playerTeam;
    Move bestMove;
    int depth;
    std::unordered_set<std::string> whitePieceLocations;
    std::unordered_set<std::string> blackPieceLocations;

    float initialEvaluation;
    float currentEvaluation;

    std::unordered_map<std::string, float> cache;
    std::string boardString;
    Team stepTeam;
    int totalCalls;
    int skippedCalls;
    int evaluations;
};

#endif