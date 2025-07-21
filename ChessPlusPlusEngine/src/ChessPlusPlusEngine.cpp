#include "pch.h"

#include "ChessPlusPlusEngine.h"
#include "game.h"
#include "engine/engine.h"

Game m_game;
Engine m_engine;

///=================================================================
bool play_move(int ox, int oy, int dx, int dy)
{
    Move move(Square(ox, oy), Square(dx, dy));
    return m_game.makeMove(move);
}

bool validate_move(int ox, int oy, int dx, int dy)
{
    Move move(Square(ox, oy), Square(dx, dy));
    return m_game.isMoveValid(move);
}

bool get_piece(int x, int y, int* team, int* type)
{
    const Piece* piece = m_game.getBoard()->getPiece(Square(x,y));

    if (piece)
    {
        *team = static_cast<int>(piece->getInfo().team);
        *type = static_cast<int>(piece->getInfo().type);

        return true;
    }

    return false;
}

bool init_game()
{
    m_game.restart();

    return true;
}

void get_next_move(int team, int* ox, int* oy, int* dx, int* dy)
{
    m_engine.setDepth(5);
    Move move = m_engine.getNextMove(&m_game);

    *ox = move.getOrigin().getX();
    *oy = move.getOrigin().getY();
    *dx = move.getDestination().getX();
    *dy = move.getDestination().getY();
}

void get_valid_moves(int x, int y, int* outMoves, int* outCount)
{
    Square origin(x, y);
    SquareVec valid = m_game.getValidMoves(origin);

    int count = 0;
    for (const Square& sq : valid)
    {
        outMoves[count * 2 + 0] = sq.getX();
        outMoves[count * 2 + 1] = sq.getY();
        count++;
    }

    *outCount = count;
}
