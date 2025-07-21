#ifndef GAME_HEADER
#define GAME_HEADER

#include "basic.h"

///=================================================================
struct MoveHistoryEvent {
    MoveType type = MoveType::IMPOSSIBLE;
    Move move;
    Team team = Team::WHITE;
    std::unique_ptr<Piece> capturedPiece = nullptr;
};

////////////////////////////////////////////////////////////////////
/// Game
///=================================================================
class Game
{
public:
    Game();

    const Board* getBoard();

    Team getCurrentTurn();
    //const MoveVec getValidMoves();
    SquareVec getValidMoves(const Square& origin);

    bool isMoveValid(const Move& move);
    bool makeMove(const Move& move);
    bool undoLastMove();

    void restart();
    //setPosition
    //const std::vector<Move> getValidMoves();

private:
    bool isSquareAttacked(const Square& square);
    bool isPathWayClear(const SquareVec& path);
    bool isPathDestinationClear(const SquareVec& path);
    bool isPathDestinationCapturable(const SquareVec& path);
    bool isPositionCheck();
    MoveType getMoveType(const Move& move);
    MoveType getMoveTypeInGameContext(const Move& move);

    bool pushMove(const MoveType& type, const Move& move);
    bool popMove();

    // moveHistory
    // state 
    // ...

    Board board;
    Team currentTurn;
    std::stack<MoveHistoryEvent> moveHistory;
};

#endif