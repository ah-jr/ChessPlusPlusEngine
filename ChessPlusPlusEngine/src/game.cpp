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
    : currentTurn(Player::WHITE)
{
}

///=================================================================
const Board* Game::getBoard()
{
    return &board;
}

///=================================================================
Player Game::getCurrentTurn()
{
    return currentTurn;
}

///=================================================================
bool Game::isMoveValid(const Move& move)
{
    if (!move.isValid())
        return false;

    const Piece* piece = board.getPiece(move.getOrigin());

    if (piece == nullptr)
        return false;

    return isMoveCurrentlyPossible(piece->getMoveInfo(move), piece->getTeam());
}

///=================================================================
bool Game::makeMove(const Move& move)
{
    if (!isMoveValid(move))
        return false;

    board.movePiece(move);
    currentTurn = GetOpponent(currentTurn);

    return true;
}

///=================================================================
void Game::restart()
{
    // White pieces
    board.putPiece(Square("a1"), Rook(Player::WHITE));
    board.putPiece(Square("b1"), Knight(Player::WHITE));
    board.putPiece(Square("c1"), Bishop(Player::WHITE));
    board.putPiece(Square("d1"), Queen(Player::WHITE));
    board.putPiece(Square("e1"), King(Player::WHITE));
    board.putPiece(Square("f1"), Bishop(Player::WHITE));
    board.putPiece(Square("g1"), Knight(Player::WHITE));
    board.putPiece(Square("h1"), Rook(Player::WHITE));

    board.putPiece(Square("a2"), Pawn(Player::WHITE));
    board.putPiece(Square("b2"), Pawn(Player::WHITE));
    board.putPiece(Square("c2"), Pawn(Player::WHITE));
    board.putPiece(Square("d2"), Pawn(Player::WHITE));
    board.putPiece(Square("e2"), Pawn(Player::WHITE));
    board.putPiece(Square("f2"), Pawn(Player::WHITE));
    board.putPiece(Square("g2"), Pawn(Player::WHITE));
    board.putPiece(Square("h2"), Pawn(Player::WHITE));

    // Black pieces
    board.putPiece(Square("a8"), Rook(Player::BLACK));
    board.putPiece(Square("b8"), Knight(Player::BLACK));
    board.putPiece(Square("c8"), Bishop(Player::BLACK));
    board.putPiece(Square("d8"), Queen(Player::BLACK));
    board.putPiece(Square("e8"), King(Player::BLACK));
    board.putPiece(Square("f8"), Bishop(Player::BLACK));
    board.putPiece(Square("g8"), Knight(Player::BLACK));
    board.putPiece(Square("h8"), Rook(Player::BLACK));

    board.putPiece(Square("a7"), Pawn(Player::BLACK));
    board.putPiece(Square("b7"), Pawn(Player::BLACK));
    board.putPiece(Square("c7"), Pawn(Player::BLACK));
    board.putPiece(Square("d7"), Pawn(Player::BLACK));
    board.putPiece(Square("e7"), Pawn(Player::BLACK));
    board.putPiece(Square("f7"), Pawn(Player::BLACK));
    board.putPiece(Square("g7"), Pawn(Player::BLACK));
    board.putPiece(Square("h7"), Pawn(Player::BLACK));
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
bool Game::isPathDestinationCapturable(const SquareVec& path, Player team)
{
    if (isPathDestinationClear(path))
        return false;

    return board.getPiece(path.back())->getTeam() != team;
}

///=================================================================
bool Game::isMoveCurrentlyPossible(const MoveInfo& info, Player team)
{
    if (team != currentTurn)
        return false;

    if (info.type == MoveType::IMPOSSIBLE)
        return false;

    // Check for checks

    switch (info.type)
    {
    case MoveType::REGULAR:
        return isPathWayClear(info.path) && (isPathDestinationClear(info.path) || isPathDestinationCapturable(info.path, team));

    case MoveType::PAWN_MOVE:
    case MoveType::PAWN_DOUBLE_MOVE:
        return isPathWayClear(info.path) && isPathDestinationClear(info.path);

    case MoveType::PAWN_CAPTURE:
        return isPathDestinationCapturable(info.path, team); // + en passant

    case MoveType::PAWN_PROMOTION:
    case MoveType::SHORT_CASTLE:
    case MoveType::LONG_CASTLE:

    default:
        return false;
    }
}
