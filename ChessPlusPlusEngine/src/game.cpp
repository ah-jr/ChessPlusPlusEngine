#include "pch.h"

#include "game.h"
#include "pieces\pawn.h"
#include "pieces\rook.h"
#include "pieces\bishop.h"
#include "pieces\knight.h"
#include "pieces\queen.h"
#include "pieces\king.h"

////////////////////////////////////////////////////////////////////
/// Game
///=================================================================
Game::Game()
    : currentTurn(Team::WHITE)
{
}

///=================================================================
Board* Game::getBoard()
{
    return &board;
}

///=================================================================
Team Game::getCurrentTurn()
{
    return currentTurn;
}

///=================================================================
SquareVec Game::getValidMoves(const Square& origin)
{
    SquareVec result;
    if (!origin.isValid())
        return result;

    const Piece* piece = board.getPiece(origin);
    if (piece == nullptr)
        return result;

    for (const Direction& dir : piece->getMoveDirections())
    {
        auto dx = dir.first;
        auto dy = dir.second;

        int x = origin.getX();
        int y = origin.getY();

        for (int j = 0; j < piece->getMoveRange(); j++)
        {
            x += dx;
            y += dy;

            Square dest(x, y);
            if (!dest.isValid())
                break;

            const Piece* capture = board.getPiece(dest);
            if (capture != nullptr)
            {
                if (capture->getInfo().team != piece->getInfo().team)
                    result.push_back(dest);

                break;
            }

            result.push_back(dest);
        }
    }

    return result;
}

///=================================================================
bool Game::isMoveValid(const Move& move)
{
    return getMoveTypeInGameContext(move) != MoveType::IMPOSSIBLE;
}

///=================================================================
bool Game::makeMove(const Move& move)
{
    if (pushMove(getMoveTypeInGameContext(move), move))
    {
        currentTurn = getOpponent(currentTurn);
        return true;
    }

    return false;
}

///=================================================================
bool Game::undoLastMove()
{
    if (popMove())
    {
        currentTurn = getOpponent(currentTurn);
        return true;
    }

    return false;
}

///=================================================================
void Game::restart()
{
    // White pieces
    board.putPiece(Square("a1"), Rook(Team::WHITE));
    board.putPiece(Square("b1"), Knight(Team::WHITE));
    board.putPiece(Square("c1"), Bishop(Team::WHITE));
    board.putPiece(Square("d1"), Queen(Team::WHITE));
    board.putPiece(Square("e1"), King(Team::WHITE));
    board.putPiece(Square("f1"), Bishop(Team::WHITE));
    board.putPiece(Square("g1"), Knight(Team::WHITE));
    board.putPiece(Square("h1"), Rook(Team::WHITE));

    board.putPiece(Square("a2"), Pawn(Team::WHITE));
    board.putPiece(Square("b2"), Pawn(Team::WHITE));
    board.putPiece(Square("c2"), Pawn(Team::WHITE));
    board.putPiece(Square("d2"), Pawn(Team::WHITE));
    board.putPiece(Square("e2"), Pawn(Team::WHITE));
    board.putPiece(Square("f2"), Pawn(Team::WHITE));
    board.putPiece(Square("g2"), Pawn(Team::WHITE));
    board.putPiece(Square("h2"), Pawn(Team::WHITE));

    // Black pieces
    board.putPiece(Square("a8"), Rook(Team::BLACK));
    board.putPiece(Square("b8"), Knight(Team::BLACK));
    board.putPiece(Square("c8"), Bishop(Team::BLACK));
    board.putPiece(Square("d8"), Queen(Team::BLACK));
    board.putPiece(Square("e8"), King(Team::BLACK));
    board.putPiece(Square("f8"), Bishop(Team::BLACK));
    board.putPiece(Square("g8"), Knight(Team::BLACK));
    board.putPiece(Square("h8"), Rook(Team::BLACK));

    board.putPiece(Square("a7"), Pawn(Team::BLACK));
    board.putPiece(Square("b7"), Pawn(Team::BLACK));
    board.putPiece(Square("c7"), Pawn(Team::BLACK));
    board.putPiece(Square("d7"), Pawn(Team::BLACK));
    board.putPiece(Square("e7"), Pawn(Team::BLACK));
    board.putPiece(Square("f7"), Pawn(Team::BLACK));
    board.putPiece(Square("g7"), Pawn(Team::BLACK));
    board.putPiece(Square("h7"), Pawn(Team::BLACK));
}

///=================================================================
bool Game::isSquareAttacked(const Square& square)
{
    DirectionSet directions{ // change this to take from the existing pieces types
        { -1, -1 },
        { -1,  0 },
        { -1,  1 },
        {  0, -1 },
        {  0,  1 },
        {  1, -1 },
        {  1,  0 },
        {  1,  1 },

        { -2, -1 },
        { -2,  1 },
        { -1, -2 },
        { -1,  2 },
        {  1, -2 },
        {  1,  2 },
        {  2, -1 },
        {  2,  1 }
    };

    for (const Direction& dir : directions)
    {
        int xStep = dir.first;
        int yStep = dir.second;

        int x = square.getX();
        int y = square.getY();

        for (int range = 1; range <= 7; range++)
        {
            x += xStep;
            y += yStep;

            Square dest(x, y);
            if (!dest.isValid())
                break;

            const Piece* piece = board.getPiece(dest);
            if (piece != nullptr)
            {
                if (piece->getInfo().team == currentTurn)
                    break;

                if (piece->doesAttackInDirection({ -xStep, -yStep }) && piece->getMoveRange() >= range)
                    return true;

                break;
            }
        }
    }

    return false;
}

///=================================================================
bool Game::isPathWayClear(const SquareVec& path)
{
    for (int i = 0; i < path.size() - 1; i++)
        if (board.getPiece(path[i]) != nullptr)
            return false;

    return true;
}

///=================================================================
bool Game::isPathDestinationClear(const SquareVec& path)
{
    return board.getPiece(path.back()) == nullptr;
}

///=================================================================
bool Game::isPathDestinationCapturable(const SquareVec& path)
{
    if (isPathDestinationClear(path))
        return false;

    return board.getPiece(path.back())->getInfo().team != currentTurn;
}

///=================================================================
bool Game::isPositionCheck()
{
    PieceInfo info;
    info.team = currentTurn;
    info.type = PieceType::KING;

    auto positions = board.getPiecePositions(info);
    if (positions->empty())
        return false;

    Square kingPosition(*positions->begin());   
    return isSquareAttacked(kingPosition);

    //Move kingToPiece(kingPosition, move.getOrigin());

    //if (kingToPiece.sameDiagonal() || kingToPiece.sameLine())
    //{
    //    int xStep = (kingToPiece.getDeltaX() == 0) ? 0 : (kingToPiece.getDeltaX() > 0 ? 1 : -1);
    //    int yStep = (kingToPiece.getDeltaY() == 0) ? 0 : (kingToPiece.getDeltaY() > 0 ? 1 : -1);

    //    int x = kingPosition.getX();
    //    int y = kingPosition.getY();

    //    for (int range = 1; range <= 7; range++)
    //    {
    //        x += xStep;
    //        y += yStep;

    //        Square dest(x, y);
    //        if (!dest.isValid())
    //            break;

    //        if (dest == move.getOrigin())
    //            continue;

    //        if (dest == move.getDestination())
    //            return false;

    //        const Piece* capture = board.getPiece(dest);
    //        if (capture != nullptr)
    //        {
    //            if (capture->getInfo().team == currentTurn)
    //                return false;

    //            if (capture->doesMoveInDirection({ xStep, yStep }) && capture->getMoveRange() >= range)
    //                return true;
    //        }
    //    }
    //}

    //return false;
}

///=================================================================
MoveType Game::getMoveType(const Move& move)
{
    if (!move.isValid())
        return MoveType::IMPOSSIBLE;

    const Piece* piece = board.getPiece(move.getOrigin());
    if (piece == nullptr || piece->getInfo().team != currentTurn)
        return MoveType::IMPOSSIBLE;

    return piece->getMoveType(move);
}

///=================================================================
MoveType Game::getMoveTypeInGameContext(const Move& move)
{
    const MoveType& type = getMoveType(move);
    if (type == MoveType::IMPOSSIBLE)
        return type;

    const SquareVec& path = board.getPiece(move.getOrigin())->getMovePath(move);
    switch (type)
    {
    case MoveType::REGULAR:
        if (!isPathWayClear(path) || (!isPathDestinationClear(path) && !isPathDestinationCapturable(path)))
            return MoveType::IMPOSSIBLE;
        break;

    case MoveType::PAWN_DOUBLE_MOVE:
        if (!isPathWayClear(path))
            return MoveType::IMPOSSIBLE;
        [[fallthrough]]

    case MoveType::PAWN_MOVE:
    case MoveType::PAWN_PROMOTION:
        if (!isPathDestinationClear(path))
            return MoveType::IMPOSSIBLE;
        break;

    case MoveType::PAWN_CAPTURE:
    case MoveType::PAWN_CAPTURE_PROMOTION:
        if (!isPathDestinationCapturable(path)) // + en passant
            return MoveType::IMPOSSIBLE;
        break;

    case MoveType::SHORT_CASTLE:
    case MoveType::LONG_CASTLE:
        break;

    default:
        break;
    }

    pushMove(type, move);
    bool putsSelfInCheck = isPositionCheck();
    popMove();
    if (!putsSelfInCheck)
        return type;

    return MoveType::IMPOSSIBLE;
}

///=================================================================
bool Game::pushMove(const MoveType& type, const Move& move)
{
    MoveHistoryEvent event;

    switch (type)
    {
    case MoveType::IMPOSSIBLE:
        return false;

    case MoveType::PAWN_CAPTURE:
    case MoveType::REGULAR:
        event.capturedPiece = board.removePiece(move.getDestination());
        [[fallthrough]];

    case MoveType::PAWN_MOVE:
    case MoveType::PAWN_DOUBLE_MOVE:
        board.movePiece(move);
        break;

    case MoveType::PAWN_CAPTURE_PROMOTION:
        event.capturedPiece = board.removePiece(move.getDestination());
        [[fallthrough]];

    case MoveType::PAWN_PROMOTION:
        board.removePiece(move.getOrigin());
        board.putPiece(move.getDestination(), Queen(currentTurn));
        break;

    case MoveType::SHORT_CASTLE:
    case MoveType::LONG_CASTLE:

    default:
        return false;
    }

    event.move = move;
    event.type = type;
    event.team = currentTurn;
    moveHistory.push(std::move(event));
    return true;
}

///=================================================================
bool Game::popMove()
{
    if (moveHistory.empty())
        return false;

    MoveHistoryEvent& event = moveHistory.top();
    board.movePiece(event.move.getOpposite());
    if (event.capturedPiece != nullptr)
        board.insertPiece(event.move.getDestination(), std::move(event.capturedPiece));

    switch (event.type)
    {
    case MoveType::REGULAR:
    case MoveType::PAWN_MOVE:
    case MoveType::PAWN_DOUBLE_MOVE:
    case MoveType::PAWN_CAPTURE:
        break;

    case MoveType::PAWN_CAPTURE_PROMOTION:
    case MoveType::PAWN_PROMOTION:
        board.putPiece(event.move.getOrigin(), Pawn(event.team));
        break;

    case MoveType::SHORT_CASTLE:
    case MoveType::LONG_CASTLE:

    default:
        break;
    }

    moveHistory.pop();
    return true;
}
