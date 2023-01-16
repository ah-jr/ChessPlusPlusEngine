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

    if (board->squares[move.x][move.y] != nullptr &&
        board->squares[move.x][move.y]->team == team)
        return false;

    if (abs(move.x - pos.x) != abs(move.y - pos.y))
        return false;

    int invX = move.x < pos.x ? -1 : 1; 
    int invY = move.y < pos.y ? -1 : 1; 

    for (int i = 1 ; i<abs(move.x - pos.x); i++)
        if (board->squares[pos.x + invX*i][pos.y + invY*i] != nullptr)
            return false;
    
    return true;
}

///=================================================================
std::vector<Square>* Bishop::getValidMoves(Board* board, Square pos)
{
    auto addSquare = [](std::vector<Square>* moves, Board* board, Team team, int valX, int valY)  
    {
        Piece * aux = board->squares[valX][valY];

        if (aux == nullptr){
            moves->push_back(Square(valX, valY));
            return false;
        }
        else if (aux->team != team){
            moves->push_back(Square(valX, valY)); 
            return true;
        }
        return true;
    };

    std::vector<Square>* moves = new std::vector<Square>();

    for (int i = 1; i <= std::min(7 - pos.x, 7 - pos.y); i++)
        if(addSquare(moves, board, team, pos.x + i, pos.y + i))
            break; 
            
    for (int i = 1; i <= std::min(7 - pos.x, pos.y); i++)
        if(addSquare(moves, board, team, pos.x + i, pos.y - i))
            break; 

    for (int i = 1; i <= std::min(pos.x, pos.y); i++)
        if(addSquare(moves, board, team, pos.x - i, pos.y - i))
            break; 

    for (int i = 1; i <= std::min(pos.x, 7 - pos.y); i++)
        if(addSquare(moves, board, team, pos.x - i, pos.y + i))
            break;       

    return moves;
}

///=================================================================
const int Bishop::getValue()
{
    return 3;
}
