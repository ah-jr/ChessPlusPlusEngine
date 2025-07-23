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
const Board* Game::getBoard()
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

    DirectionSet attacks = piece->getAttackDirections();
    DirectionSet moves = piece->getMoveDirections();
    
    std::vector<std::pair<const DirectionSet&, bool>> allDirections = {
        {attacks, true},
        {moves, false}
    };

    for (auto& dir : allDirections)
    {
        const DirectionSet& directions = dir.first;
        bool isAttack = dir.second;

        for (const Direction& dir : directions)
        {
            int dx = dir.first;
            int dy = dir.second;

            int x = origin.getX();
            int y = origin.getY();

            for (int j = 0; j < piece->getMoveRange(); j++)
            {
                x += dx;
                y += dy;

                Square dest(x, y);
                if (!dest.isValid())
                    break;

                const Piece* target = board.getPiece(dest);
                if (target != nullptr)
                {
                    if (isAttack && target->getInfo().team != piece->getInfo().team)
                        result.push_back(dest);
                    break;
                }

                result.push_back(dest);
            }
        }
    }

    return result;
}

///=================================================================
float Game::getPieceBalance()
{
    float evaluation = 0.f;

    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            const Piece* piece = board.getPiece(Square(x, y));
            if (piece != nullptr)
                evaluation += piece->getValue() * (piece->getInfo().team == Team::WHITE ? 1 : -1);
        }
    }

    return evaluation;
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
    while (!moveHistory.empty())
        moveHistory.pop();

    shortCastleStatus[Team::WHITE] = true;
    shortCastleStatus[Team::BLACK] = true;
    longCastleStatus[Team::WHITE] = true;
    longCastleStatus[Team::BLACK] = true;

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
bool Game::isPathAttacked(const SquareVec& path)
{
    for (int i = 0; i < path.size(); i++)
        if (isSquareAttacked(path[i]))
            return true;

    return false;
}

///=================================================================
bool Game::isPathDestinationCapturable(const SquareVec& path)
{
    if (isPathDestinationClear(path))
        return false;

    return board.getPiece(path.back())->getInfo().team != currentTurn;
}

///=================================================================
bool Game::isValidEnPassant(const Move& move)
{
    if (moveHistory.empty())
        return false;

    const MoveHistoryEvent& lastMove = moveHistory.top();
    
    if (lastMove.type == MoveType::PAWN_DOUBLE_MOVE)
    {
        const Square& lastMoveSquare = lastMove.move.getDestination();

        if (currentTurn == Team::WHITE)
            return move.getDestination() == Square(lastMoveSquare.getX(), lastMoveSquare.getY() + 1);
        else
            return move.getDestination() == Square(lastMoveSquare.getX(), lastMoveSquare.getY() - 1);
    }

    return false;
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
    MoveType type = getMoveType(move);
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
        if (isValidEnPassant(move))
        {
            type = MoveType::PAWN_CAPTURE_EN_PASSANT;
            break;
        }
        [[fallthrough]]

    case MoveType::PAWN_CAPTURE_PROMOTION:
        if (!isPathDestinationCapturable(path))
            return MoveType::IMPOSSIBLE;
        break;

    case MoveType::SHORT_CASTLE:
        if (!shortCastleStatus[currentTurn])
            return MoveType::IMPOSSIBLE;

        if (isSquareAttacked(move.getOrigin()) || isPathAttacked(path) || !isPathDestinationClear(path) || !isPathWayClear(path))
            return MoveType::IMPOSSIBLE;

        break;

    case MoveType::LONG_CASTLE:
        if (!longCastleStatus[currentTurn])
            return MoveType::IMPOSSIBLE;

        if (isSquareAttacked(move.getOrigin()) || isPathAttacked(path) || !isPathDestinationClear(path) || !isPathWayClear(path))
            return MoveType::IMPOSSIBLE;

        // The square next to the rook must also be empty
        if (board.getPiece(Square(1, (currentTurn == Team::WHITE ? 0 : 7))) != nullptr)
            return MoveType::IMPOSSIBLE;

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
    int castleRow = (currentTurn == Team::WHITE ? 0 : 7);

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

    case MoveType::PAWN_CAPTURE_EN_PASSANT:
    {
        int captureY = move.getDestination().getY() + (currentTurn == Team::WHITE ? -1 : 1);
        Square captureSquare = Square(move.getDestination().getX(), captureY);
        event.capturedPiece = board.removePiece(captureSquare);
        board.movePiece(move);
        break;
    }

    case MoveType::PAWN_CAPTURE_PROMOTION:
        event.capturedPiece = board.removePiece(move.getDestination());
        [[fallthrough]];

    case MoveType::PAWN_PROMOTION:
        board.removePiece(move.getOrigin());
        board.putPiece(move.getDestination(), Queen(currentTurn));
        break;

    case MoveType::SHORT_CASTLE:
    { 
        Square rookOrigin = Square(7, castleRow);
        Square rookDest = Square(5, castleRow);
        board.movePiece(Move(rookOrigin, rookDest));
        board.movePiece(move);
        break;
    }
    case MoveType::LONG_CASTLE:
    {
        Square rookOrigin = Square(0, castleRow);
        Square rookDest = Square(3, castleRow);
        board.movePiece(Move(rookOrigin, rookDest));
        board.movePiece(move);
        break;
    }

    default:
        return false;
    }

    event.disableShortCastle = false;
    event.disableLongCastle = false;

    const Piece* piece = board.getPiece(move.getDestination());
    if (piece->getInfo().type == PieceType::KING)
    {
        if (shortCastleStatus[currentTurn])
        {
            shortCastleStatus[currentTurn] = false;
            event.disableShortCastle = true;
        }

        if (longCastleStatus[currentTurn])
        {
            longCastleStatus[currentTurn] = false;
            event.disableLongCastle = true;
        }
    }
    else if (piece->getInfo().type == PieceType::ROOK)
    {
        if (move.getOrigin() == Square(7, castleRow) && shortCastleStatus[currentTurn])
        {
            shortCastleStatus[currentTurn] = false;
            event.disableShortCastle = true;
        }
        else if (move.getOrigin() == Square(0, castleRow) && longCastleStatus[currentTurn])
        {
            longCastleStatus[currentTurn] = false;
            event.disableLongCastle = true;
        }
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

    int castleRow = (currentTurn == Team::WHITE ? 0 : 7);

    MoveHistoryEvent& event = moveHistory.top();
    board.movePiece(event.move.getOpposite());
    if (event.capturedPiece != nullptr)
        board.putPiece(event.move.getDestination(), std::move(event.capturedPiece));

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
    {
        Square rookOrigin = Square(5, castleRow);
        Square rookDest = Square(7, castleRow);
        board.movePiece(Move(rookOrigin, rookDest));
        break;
    }
    case MoveType::LONG_CASTLE:
    {
        Square rookOrigin = Square(3, castleRow);
        Square rookDest = Square(0, castleRow);
        board.movePiece(Move(rookOrigin, rookDest));
        break;
    }

    default:
        break;
    }

    if (event.disableShortCastle)
        shortCastleStatus[currentTurn] = true;

    if (event.disableLongCastle)
        longCastleStatus[currentTurn] = true;

    moveHistory.pop();
    return true;
}
