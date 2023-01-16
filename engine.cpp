#include "engine.h"
#include <vector>
#include <climits>
#include <algorithm>

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
    int cur_x;

    for (auto it_p = selected->begin(); it_p != selected->end(); ++it_p) {
        Piece* piece = board.squares[it_p->x][it_p->y];
        moves = piece->getValidMoves(&board);

        for (auto it_m = moves->begin(); it_m != moves->end(); ++it_m) {
            Piece* piece = board.squares[it_m->x][it_m->y];

            cur_eval = piece != nullptr ? piece->getValue() : 0;

            if (depth != 1){
                cur_eval -= getNextMoveR(Move(Square(it_p->x, it_p->y), Square(it_m->x, it_m->y)), board, GetOppositeTeam(team), depth - 1);
            }

            if (cur_eval > evaluation || (cur_eval == evaluation && abs(3 - cur_x) > abs(3 - it_p->x))){
                origin = Square(it_p->x, it_p->y);
                dest   = Square(it_m->x, it_m->y);
                evaluation = cur_eval;
                cur_x = it_p->x;
            }   

        }
    }
    delete whitePieces, blackPieces;
    if (moves) delete moves;

    return Move(origin, dest);
}

///=================================================================
int Engine::getNextMoveR(const Move& move, Board board, Team team, int depth)
{
    std::vector<Square>* whitePieces = new std::vector<Square>();
    std::vector<Square>* blackPieces = new std::vector<Square>();
    std::vector<Square>* selected = team == Team::WHITE ? whitePieces : blackPieces;
    std::vector<Square>* moves = nullptr;

    whitePieces->clear();
    blackPieces->clear();

    board.squares[move.d.x][move.d.y] = board.squares[move.o.x][move.o.y];
    board.squares[move.o.x][move.o.y] = nullptr;

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

    int evaluation = INT_MIN;
    int cur_eval;

    for (auto it_p = selected->begin(); it_p != selected->end(); ++it_p) {
        Piece* piece = board.squares[it_p->x][it_p->y];
        moves = piece->getValidMoves(&board);

        for (auto it_m = moves->begin(); it_m != moves->end(); ++it_m) {
            Piece* piece = board.squares[it_m->x][it_m->y];

            cur_eval = piece != nullptr ? piece->getValue() : 0;

            if (depth != 1){
                cur_eval -= getNextMoveR(Move(Square(it_p->x, it_p->y), Square(it_m->x, it_m->y)), board, GetOppositeTeam(team), depth - 1);
            }

            if (cur_eval > evaluation)
                evaluation = cur_eval;
        }
    }
    delete whitePieces, blackPieces;
    if (moves) delete moves;

    return evaluation;
}