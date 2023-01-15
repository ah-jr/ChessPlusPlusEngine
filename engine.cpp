#include "engine.h"
#include <vector>

////////////////////////////////////////////////////////////////////
/// Engine
///=================================================================
Engine::Engine(Board* board)
    : m_board(board)
{
}

///=================================================================
Square Engine::getNextMove(Team team, int depth)
{
    std::vector<Square>* whitePieces;
    std::vector<Square>* blackPieces;
    std::vector<Square>* selected = team == Team::WHITE ? whitePieces : blackPieces;

    for(int i = 0; i < 7; i++){
        for(int j = 0; j < 7; j++){
            if (m_board->squares[i][j] != nullptr) {
                if (m_board->squares[i][j]->team == Team::WHITE)
                    whitePieces->push_back(Square(i, j));    
                if (m_board->squares[i][j]->team == Team::BLACK)
                    blackPieces->push_back(Square(i, j));                  
            }
        }
    }

    for (auto it_p = selected->begin(); it_p != selected->end(); ++it_p) {
        Piece* piece = m_board->squares[it_p->x][it_p->y];
        std::vector<Square>* moves = piece->getValidMoves(m_board);
        int evaluation = 0;

        for (auto it_m = moves->begin(); it_m != moves->end(); ++it_m) {
            Piece* piece = m_board->squares[it_m->x][it_m->y];
            if (piece != nullptr)
                evaluation += piece->getValue();
        }
    }
    
    delete whitePieces, blackPieces;              
}