#ifndef GAME_HEADER
#define GAME_HEADER

#include "basic.h"

////////////////////////////////////////////////////////////////////
/// Game
///=================================================================
class Game
{
public:
    Game();

    const Board* getBoard();

    Player getCurrentTurn();
    //const MoveVec getValidMoves();
    //const MoveVec getValidMoves(const Piece* piece);

    bool isMoveValid(const Move& move);
    bool makeMove(const Move& move);

    void restart();
    //setPosition
    //const std::vector<Move> getValidMoves();

private:
    bool isPathWayClear(const SquareVec& path);
    bool isPathDestinationClear(const SquareVec& path);
    bool isPathDestinationCapturable(const SquareVec& path, Player team);
    bool isMoveCurrentlyPossible(const MoveInfo& info, Player team);

    // moveHistory
    // state 
    // ...

    Board board;
    Player currentTurn;
};

#endif