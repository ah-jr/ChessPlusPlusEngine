#include <iostream>
#include "chess_main.h"
#include "pieces\base.h"
#include "pieces\pawn.h"
#include "pieces\rook.h"
#include "pieces\bishop.h"
#include "pieces\knight.h"
#include "pieces\queen.h"
#include "pieces\king.h"

void printIsValid(Piece* piece, Square move, Board* board)
{
    std::cout << "Move piece of type (" << int(piece->getType()) << ") to " << move.x << ":" << move.y << " is: ";
    std::cout << (piece->checkValidMove(move, board) ? "Valid\n" : "Invalid\n"); 
}

////////////////////////////////////////////////////////////////////
/// Main
///=================================================================
int chess_test()
{
    Piece* pawn1 = new Pawn(Team::WHITE);
    Piece* pawn2 = new Pawn(Team::BLACK);
    Piece* rook1 = new Rook(Team::BLACK);
    Piece* bishop1 = new Bishop(Team::BLACK);

    Board* board = new Board();
    board->squares[1][1] = pawn1;
    board->squares[6][6] = pawn2;
    board->squares[5][1] = rook1;
    board->squares[3][3] = bishop1;

    printIsValid(bishop1, Square(2, 2), board);
    printIsValid(bishop1, Square(1, 1), board);
    printIsValid(bishop1, Square(5, 5), board);
    printIsValid(bishop1, Square(6, 6), board);
    printIsValid(bishop1, Square(4, 2), board);
    printIsValid(bishop1, Square(5, 1), board);

    delete pawn1, pawn2, rook1, bishop1, board;

    return 0;
}
