#ifndef GAME_HEADER
#define GAME_HEADER

#include "basic.h"

///=================================================================
struct MoveHistoryEvent {
    MoveType type = MoveType::IMPOSSIBLE;
    Move move;
    Team team = Team::WHITE;
    bool disableShortCastle;
    bool disableLongCastle;
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
    SquareVec getValidMoves(const Square& origin);
    float getPieceBalance();

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
    bool isPathAttacked(const SquareVec& path);
    bool isPathDestinationCapturable(const SquareVec& path);
    bool isValidEnPassant(const Move& move);
    bool isPositionCheck();

    MoveType getMoveType(const Move& move);
    MoveType getMoveTypeInGameContext(const Move& move);

    bool pushMove(const MoveType& type, const Move& move);
    bool popMove();

    Board board;
    Team currentTurn;
    std::stack<MoveHistoryEvent> moveHistory;

    std::map<Team, bool> longCastleStatus;
    std::map<Team, bool> shortCastleStatus;
};

#endif