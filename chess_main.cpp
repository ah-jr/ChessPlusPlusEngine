#include <iostream>
#include "chess_main.h"
#include "engine.h"
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

Board* m_board;
Engine* m_engine;


////////////////////////////////////////////////////////////////////
/// Main
///=================================================================
int chess_test()
{
    init_game();
    Move move = m_engine->getNextMove(m_board, Team::WHITE, 4);

    std::cout << move.o.x << ":" << move.o.y << " -> "  << move.d.x << ":"  << move.d.y;

    return 0;
}

bool play_move(int ox, int oy, int dx, int dy)
{
    if (!m_board)
        return false;

    if (m_board->squares[ox][oy] == nullptr)
        return false;
    
    if (!m_board->squares[ox][oy]->checkValidMove(Square(dx, dy), m_board))
        return false;

    if (m_board->squares[dx][dy] != nullptr)
        delete m_board->squares[dx][dy];

    m_board->squares[dx][dy] = m_board->squares[ox][oy];
    m_board->squares[ox][oy] = nullptr;

    // Castles
    if (m_board->squares[dx][dy]->getType() == PieceType::ROOK){
        if (m_board->squares[dx][dy]->team == Team::WHITE){
            if (ox == 0 && oy == 0)
                m_board->whiteCastleL = false;
            else if (ox == 7 && oy == 0)
                m_board->whiteCastleR = false;
        }
        else{
            if (ox == 0 && oy == 7)
                m_board->blackCastleL = false;
            else if (ox == 7 && oy == 7)
                m_board->blackCastleR = false;
        }
    }

    if (m_board->squares[dx][dy]->getType() == PieceType::KING){
        if (m_board->squares[dx][dy]->team == Team::WHITE){
            m_board->whiteCastleL = false; 
            m_board->whiteCastleR = false;

            if (ox > dx + 1){
                m_board->squares[3][0] = m_board->squares[0][0];
                m_board->squares[0][0] = nullptr;
            }
            else if (ox + 1 < dx){
                m_board->squares[5][0] = m_board->squares[7][0];
                m_board->squares[7][0] = nullptr; 
            }           
        }
        else {
            m_board->blackCastleL = false; 
            m_board->blackCastleR = false;        

            if (ox > dx + 1){
                m_board->squares[3][7] = m_board->squares[0][7];
                m_board->squares[0][7] = nullptr;
            }
            else if (ox + 1 < dx){
                m_board->squares[5][7] = m_board->squares[7][7];
                m_board->squares[7][7] = nullptr;
            }        
        }
    }

    return true; 
}

bool validate_move(int ox, int oy, int dx, int dy)
{
    if (!m_board)
        return false;

    if (m_board->squares[ox][oy] == nullptr)
        return false;
    
    return m_board->squares[ox][oy]->checkValidMove(Square(dx, dy), m_board);   
}

bool get_piece(int x, int y, int* team, int* type)
{
    Piece* piece = m_board->squares[x][y];

    if (piece)
    {
        *team = static_cast<int>(piece->team);
        *type = static_cast<int>(piece->getType());

        return true;
    }

    return false;
}

bool init_game()
{
    m_board = new Board();
    m_engine = new Engine(m_board);

    // White pieces
    m_board->squares[0][1] = new Pawn(Team::WHITE);
    m_board->squares[1][1] = new Pawn(Team::WHITE);
    m_board->squares[2][1] = new Pawn(Team::WHITE);
    m_board->squares[3][1] = new Pawn(Team::WHITE);
    m_board->squares[4][1] = new Pawn(Team::WHITE);
    m_board->squares[5][1] = new Pawn(Team::WHITE);
    m_board->squares[6][1] = new Pawn(Team::WHITE);
    m_board->squares[7][1] = new Pawn(Team::WHITE);

    m_board->squares[0][0] = new Rook  (Team::WHITE);
    m_board->squares[1][0] = new Knight(Team::WHITE);
    m_board->squares[2][0] = new Bishop(Team::WHITE);
    m_board->squares[3][0] = new Queen (Team::WHITE);
    m_board->squares[4][0] = new King  (Team::WHITE);
    m_board->squares[5][0] = new Bishop(Team::WHITE);
    m_board->squares[6][0] = new Knight(Team::WHITE);
    m_board->squares[7][0] = new Rook  (Team::WHITE);

    // Black pieces
    m_board->squares[0][6] = new Pawn(Team::BLACK);
    m_board->squares[1][6] = new Pawn(Team::BLACK);
    m_board->squares[2][6] = new Pawn(Team::BLACK);
    m_board->squares[3][6] = new Pawn(Team::BLACK);
    m_board->squares[4][6] = new Pawn(Team::BLACK);
    m_board->squares[5][6] = new Pawn(Team::BLACK);
    m_board->squares[6][6] = new Pawn(Team::BLACK);
    m_board->squares[7][6] = new Pawn(Team::BLACK);

    m_board->squares[0][7] = new Rook  (Team::BLACK);
    m_board->squares[1][7] = new Knight(Team::BLACK);
    m_board->squares[2][7] = new Bishop(Team::BLACK);
    m_board->squares[3][7] = new Queen (Team::BLACK);
    m_board->squares[4][7] = new King  (Team::BLACK);
    m_board->squares[5][7] = new Bishop(Team::BLACK);
    m_board->squares[6][7] = new Knight(Team::BLACK);
    m_board->squares[7][7] = new Rook  (Team::BLACK);

    return true;
}

void get_next_move(int team, int* ox, int* oy, int* dx, int* dy)
{
    Move move = m_engine->getNextMove(m_board, static_cast<Team>(team), 6);

    *ox = move.o.x;
    *oy = move.o.y;
    *dx = move.d.x;
    *dy = move.d.y;
}