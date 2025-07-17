#include "base.h"

////////////////////////////////////////////////////////////////////
/// Piece
///=================================================================
Piece::Piece(Team team, PieceType type)
{
    this->team = team;
    this->type = type;
}

///=================================================================
PieceType Piece::getType()
{
    return this->type;
}

///=================================================================
bool Piece::checkValidSquare(Square square)
{
    return square.x >= 0 && square.x <= 7 && square.y >=0 && square.y <= 7;
}

////////////////////////////////////////////////////////////////////
/// Team
///=================================================================
Team GetOppositeTeam(Team team)
{
    return team == Team::WHITE ? Team::BLACK : Team::WHITE; 
}

////////////////////////////////////////////////////////////////////
/// Board
///=================================================================
Piece* Board::getPiece(Square square){
    return squares[square.x][square.y];
}

///=================================================================
Square Board::checkIfContainsPiece(Piece* piece){
    for (int i=0; i<8; i++)
        for (int j=0; j<8; j++)
            if (squares[i][j] == piece)
                return Square(i, j);
    return Square(-1, -1);
}

///=================================================================
bool Board::checkIfOutOfBounds(Square square){
    return (square.x < 0 || square.x > 7 || square.y < 0 || square.y > 7);
}

///=================================================================
int Board::countAttacks(Square square, Team team){
    int attacks = 0;

    for (int i=0; i<8; i++)
        for (int j=0; j<8; j++)
            if (squares[i][j] && 
                squares[i][j]->team == GetOppositeTeam(team) && 
                squares[i][j]->checkValidMove(square, this)) 
            {
                // Pawns only capture sideways, that must be considered
                if (squares[i][j]->getType() != PieceType::PAWN || (square.x != i))
                    attacks++;
            }
    return attacks;        
}