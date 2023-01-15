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
    if (board->checkIfOutOfBounds(move))
        return false;

    Square pos = board->checkIfContainsPiece(this);
    if (pos.x == -1)
        return false;   

    if (board->squares[move.x][move.y] != nullptr &&
        board->squares[move.x][move.y]->team == team)
        return false;

    if (abs(pos.x - move.x) > 1 || abs(pos.y - move.y) > 1)
        return false;
    
    return true;
}

///=================================================================
std::vector<Square>* King::getValidMoves(Board* board)
{
    std::vector<Square>* moves = new std::vector<Square>();
    Square pos = board->checkIfContainsPiece(this);

    for(int i = pos.x - 1; i <= pos.x + 1; i++){
        for(int j = pos.y - 1; j <= pos.y + 1; j++){
            if (checkValidSquare(Square(i, j))) {
                if (board->squares[i][j] == nullptr)
                    moves->push_back(Square(i, j));
                else if (board->squares[i][j]->team != team)
                    moves->push_back(Square(i, j)); 
            }           
        }
    }

    return moves;        
}

///=================================================================
const int King::getValue()
{
    return INT_MAX;
}
