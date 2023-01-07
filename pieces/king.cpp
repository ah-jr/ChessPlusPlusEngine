#include "king.h"
#include <algorithm>

////////////////////////////////////////////////////////////////////
/// Pawn
///=================================================================
King::King(Team team) 
    : Piece(team, PieceType::KING)
{
}

///=================================================================
bool King::checkValidMove(Square move, Board* board)
{
    Square pos = board->checkIfContainsPiece(this);

    if (pos.x == -1)
        return false;    

    Team opponent;
    int start, middle, end, diff;

    if (this->team == Team::WHITE)
    {
        opponent = Team::BLACK;
        start = 1;
        middle = 2;
        end = 3;
        diff = 1;
    }
    else
    {
        opponent = Team::WHITE;
        start = 6;
        middle = 5;
        end = 4;
        diff = -1;
    }

    if (move.x != pos.x) 
    {
        return (abs(pos.x - move.x) == 1 && 
                board->squares[move.x][move.y] != nullptr &&
                board->squares[move.x][move.y]->team == opponent);
    }
    else
    {
        if (pos.y == start && 
            move.y == end && 
            board->squares[move.x][middle] == nullptr && 
            board->squares[move.x][end] == nullptr) 
            return true;
        else if (move.y - pos.y == diff && 
                board->squares[move.x][move.y] == nullptr)
            return true;
        else    
            return false;
    }

    return false;
}
