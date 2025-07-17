#include "pch.h"

#include "queen.h"
#include <algorithm>

////////////////////////////////////////////////////////////////////
/// Queen
///=================================================================
Queen::Queen(Team team) 
    : Piece(team, PieceType::QUEEN)
{
}

///=================================================================
bool Queen::checkValidMove(Square move, Board* board)
{
    if (board->checkIfOutOfBounds(move))
        return false;

    Square pos = board->checkIfContainsPiece(this);
    if (pos.x == -1)
        return false;   

    if (board->squares[move.x][move.y] != nullptr &&
        board->squares[move.x][move.y]->team == team)
        return false;

    bool sameDiag = (abs(move.x - pos.x) == abs(move.y - pos.y));
    bool sameLine = (pos.x == move.x || pos.y == move.y);

    if (!sameDiag && !sameLine)
        return false;

    bool vert = pos.x == move.x;
    int  dist = vert ? abs(move.y - pos.y) : abs(move.x - pos.x);
    int  invX = move.x < pos.x ? -1 : 1; 
    int  invY = move.y < pos.y ? -1 : 1; 

    if (sameDiag)
    {
        for (int i = 1 ; i<abs(move.x - pos.x); i++)
            if (board->squares[pos.x + invX*i][pos.y + invY*i] != nullptr)
                return false;
    }

    if (sameLine)
    {
        for (int i = 1; i < dist; i++)
        {
            int X = pos.x + (vert ? 0 : invX*i);
            int Y = pos.y + (vert ? invY*i : 0); 

            if (board->squares[X][Y] != nullptr)
                return false;   
        }
    }

    return true;
}

///=================================================================
std::vector<Square>* Queen::getValidMoves(Board* board, Square pos)
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

    for (int i = 1; i <= 7 - pos.x; i++)
        if(addSquare(moves, board, team, pos.x + i, pos.y))
            break; 
            
    for (int i = 1; i <= pos.x; i++)
        if(addSquare(moves, board, team, pos.x - i, pos.y))
            break; 

    for (int i = 1; i <= 7 - pos.y; i++)
        if(addSquare(moves, board, team, pos.x, pos.y + i))
            break; 

    for (int i = 1; i <= pos.y; i++)
        if(addSquare(moves, board, team, pos.x, pos.y - i))
            break;       

    return moves;
}

///=================================================================
const int Queen::getValue()
{
    return 9;
}
