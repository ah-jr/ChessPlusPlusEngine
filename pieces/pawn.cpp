#include "pawn.h"
#include <algorithm>

////////////////////////////////////////////////////////////////////
/// Pawn
///=================================================================
Pawn::Pawn(Team team) 
    : Piece(team, PieceType::PAWN)
{
}

///=================================================================
bool Pawn::checkValidMove(Square move, Board* board)
{
    if (board->checkIfOutOfBounds(move))
        return false;

    Square pos = board->checkIfContainsPiece(this);

    if (pos.x == -1)
        return false;    

    Team opponent = this->team == Team::WHITE ? Team::BLACK : Team::WHITE;
    int start     = this->team == Team::WHITE ? 1 : 6;
    int middle    = this->team == Team::WHITE ? 2 : 5;    
    int end       = this->team == Team::WHITE ? 3 : 4;
    int diff      = this->team == Team::WHITE ? 1 : -1;

    if (move.x != pos.x) 
    {
        return (abs(pos.x - move.x) == 1 && 
                move.y - pos.y == diff && 
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
