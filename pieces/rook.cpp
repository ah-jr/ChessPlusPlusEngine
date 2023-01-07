#include "rook.h"
#include <algorithm>

////////////////////////////////////////////////////////////////////
/// Rook
///=================================================================
Rook::Rook(Team team) 
    : Piece(team, PieceType::ROOK)
{
}

///=================================================================
bool Rook::checkValidMove(Square move, Board* board)
{
    if (board->checkIfOutOfBounds(move))
        return false;

    Square pos = board->checkIfContainsPiece(this);

    if (pos.x == -1)
        return false;   

    Team opponent = this->team == Team::WHITE ? Team::BLACK : Team::WHITE;

    if (pos.x != move.x && pos.y != move.y)
        return false;

    if (board->squares[move.x][move.y] != nullptr &&
        board->squares[move.x][move.y]->team == team)
        return false;

    int sq1 = pos.x == move.x ? move.y : move.x;
    int sq2 = pos.x == move.x ? pos.y : pos.x;

    if (sq2 < sq1)
        std::swap(sq1, sq2);

    for (int i=sq1+1; i<sq2; i++)
        if (board->squares[pos.x][i] != nullptr)
            return false;
    
    return true;
}
