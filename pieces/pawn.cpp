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

///=================================================================
std::vector<Square>* Pawn::getValidMoves(Board* board)
{
    std::vector<Square>* moves = new std::vector<Square>();
    Square pos = board->checkIfContainsPiece(this);
    Square aux;

    int diff     = (team == Team::WHITE) ? 1 : -1;
    int startPos = (team == Team::WHITE) ? 1 : 6;

    aux = Square(pos.x - 1, pos.y + diff);
    if (checkValidSquare(aux))
        if (board->squares[aux.x][aux.y] != nullptr &&
            board->squares[aux.x][aux.y]->team != team)
            moves->push_back(aux);  

    aux = Square(pos.x + 1, pos.y + diff);
    if (checkValidSquare(aux))
        if (board->squares[aux.x][aux.y] != nullptr &&
            board->squares[aux.x][aux.y]->team != team)
            moves->push_back(aux);   

    aux = Square(pos.x, pos.y + diff);
    if (checkValidSquare(aux)){
        if (board->squares[aux.x][aux.y] == nullptr)
            moves->push_back(aux);   
    } 

    aux = Square(pos.x, pos.y + 2*diff);
    if (checkValidSquare(aux)){
        if (board->squares[aux.x][aux.y] == nullptr && pos.y == startPos)
            moves->push_back(aux);   
    } 

    return moves;
}

///=================================================================
const int Pawn::getValue()
{
    return 1;
}
