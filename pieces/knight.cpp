#include "knight.h"
#include <algorithm>

////////////////////////////////////////////////////////////////////
/// Knight
///=================================================================
Knight::Knight(Team team) 
    : Piece(team, PieceType::KNIGHT)
{
}

///=================================================================
bool Knight::checkValidMove(Square move, Board* board)
{
    if (board->checkIfOutOfBounds(move))
        return false;

    Square pos = board->checkIfContainsPiece(this);
    if (pos.x == -1)
        return false;   

    if (board->squares[move.x][move.y] != nullptr &&
        board->squares[move.x][move.y]->team == team)
        return false;

    if (abs(pos.x - move.x) == 1 && abs(pos.y - move.y) == 2 ||
        abs(pos.x - move.x) == 2 && abs(pos.y - move.y) == 1)
        return true;
    
    return false;
}

///=================================================================
std::vector<Square>* Knight::getValidMoves(Board* board, Square pos)
{
    auto addSquare = [](std::vector<Square>* moves, Board* board, Team team, Square aux)  
    {
        if (board->squares[aux.x][aux.y] == nullptr)
            moves->push_back(aux);
        else if (board->squares[aux.x][aux.y]->team != team)
            moves->push_back(aux); 
    };

    std::vector<Square>* moves = new std::vector<Square>();
    Square aux;

    aux = Square(pos.x + 1, pos.y + 2);
    if (checkValidSquare(aux))
        addSquare(moves, board, team, aux);

    aux = Square(pos.x + 1, pos.y - 2);
    if (checkValidSquare(aux))
        addSquare(moves, board, team, aux);  

    aux = Square(pos.x - 1, pos.y + 2);
    if (checkValidSquare(aux))
        addSquare(moves, board, team, aux);

    aux = Square(pos.x - 1, pos.y - 2);
    if (checkValidSquare(aux))
        addSquare(moves, board, team, aux);     

    aux = Square(pos.x + 2, pos.y + 1);
    if (checkValidSquare(aux))
        addSquare(moves, board, team, aux);

    aux = Square(pos.x + 2, pos.y - 1);
    if (checkValidSquare(aux))
        addSquare(moves, board, team, aux);  

    aux = Square(pos.x - 2, pos.y + 1);
    if (checkValidSquare(aux))
        addSquare(moves, board, team, aux);

    aux = Square(pos.x - 2, pos.y - 1);
    if (checkValidSquare(aux))
        addSquare(moves, board, team, aux);     

    return moves;
}

///=================================================================
const int Knight::getValue()
{
    return 3;
}
