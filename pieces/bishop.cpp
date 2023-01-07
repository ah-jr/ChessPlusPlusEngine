#include "bishop.h"
#include <algorithm>

////////////////////////////////////////////////////////////////////
/// Bishop
///=================================================================
Bishop::Bishop(Team team) 
    : Piece(team, PieceType::BISHOP)
{
}

///=================================================================
bool Bishop::checkValidMove(Square move, Board* board)
{
    if (board->checkIfOutOfBounds(move))
        return false;

    Square pos = board->checkIfContainsPiece(this);

    if (pos.x == -1)
        return false;   

    Team opponent = this->team == Team::WHITE ? Team::BLACK : Team::WHITE;

    if (pos.x == move.x || pos.y == move.y)
        return false;

    if (board->squares[move.x][move.y] != nullptr &&
        board->squares[move.x][move.y]->team == team)
        return false;

    if (abs(move.x - pos.x) != abs(move.y - pos.y))
        return false;

    bool xBigger = move.x < pos.x;
    bool yBigger = move.y < pos.y;

    int sq1 = xBigger ? move.x : pos.x;
    int sq2 = xBigger ? pos.x : move.x;
    int inv = xBigger ^ yBigger ? -1 : 1;

    for (int i=sq1+1; i<sq2; i++)
        if (board->squares[i][pos.y+inv*(i-sq1)] != nullptr)
            return false;
    
    return true;
}
