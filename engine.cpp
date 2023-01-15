#include "engine.h"
#include <vector>
#include <climits>

////////////////////////////////////////////////////////////////////
/// Engine
///=================================================================
Engine::Engine(Board* board)
    : m_board(board)
{
}

///=================================================================
Move Engine::getNextMove(Board board, Team team, int depth)
{
    std::vector<Square>* whitePieces = new std::vector<Square>();
    std::vector<Square>* blackPieces = new std::vector<Square>();
    std::vector<Square>* selected = team == Team::WHITE ? whitePieces : blackPieces;
    std::vector<Square>* moves = nullptr;

    whitePieces->clear();
    blackPieces->clear();

    for(int i = 0; i <= 7; i++){
        for(int j = 0; j <= 7; j++){
            if (board.squares[i][j] != nullptr) {
                if (board.squares[i][j]->team == Team::WHITE)
                    whitePieces->push_back(Square(i, j));    
                if (board.squares[i][j]->team == Team::BLACK)
                    blackPieces->push_back(Square(i, j));                  
            }
        }
    }

    Square origin = Square(-1, -1);
    Square dest = Square(-1, -1);
    int evaluation = INT_MIN;
    int cur_eval;
//    bool set = false;

    for (auto it_p = selected->begin(); it_p != selected->end(); ++it_p) {
        Piece* piece = board.squares[it_p->x][it_p->y];
        moves = piece->getValidMoves(&board);

        for (auto it_m = moves->begin(); it_m != moves->end(); ++it_m) {
            // if (!set){
            //     origin = Square(it_p->x, it_p->y);
            //     dest   = Square(it_m->x, it_m->y);                
            //     set    = true;
            // }

            Piece* piece = board.squares[it_m->x][it_m->y];

            if (depth == 1){
                if (piece != nullptr)
                    cur_eval = piece->getValue();
                else
                    cur_eval = 0;

                if (cur_eval > evaluation){
                    origin = Square(it_p->x, it_p->y);
                    dest   = Square(it_m->x, it_m->y);
                    evaluation = cur_eval;
                }                
            }


            // if (piece != nullptr){
            //     if (piece->getValue() > evaluation){
            //         origin = Square(it_p->x, it_p->y);
            //         dest   = Square(it_m->x, it_m->y);
            //         evaluation = piece->getValue();
            //     }
            // }
        }
    }
    delete whitePieces, blackPieces;
    if (moves) delete moves;

    return Move(origin, dest);
}