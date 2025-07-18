#include "pch.h"

#include "ChessPlusPlusEngine.h"
#include "game.h"

Game m_game;

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
        *team = static_cast<int>(piece->getTeam());
        *type = static_cast<int>(piece->getType());

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
    //Move move = m_engine->getNextMove(m_board, static_cast<Player>(team), 6);

    //*ox = move.o.x;
    //*oy = move.o.y;
    //*dx = move.d.x;
    //*dy = move.d.y;
}