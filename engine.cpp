#include "engine.h"
#include <vector>
#include <climits>
#include <algorithm>
#include <float.h>

////////////////////////////////////////////////////////////////////
/// Engine
///=================================================================
Engine::Engine(Board* board)
    : m_board(board)
{
}

///=================================================================
Move Engine::getNextMove(Board* board, Team team, int depth)
{
    std::vector<Square>* moves = nullptr;
    Square origin = Square(-1, -1);
    Square dest = Square(-1, -1);
    float evaluation = FLT_MIN;
    float cur_eval;
    int lastx = 0;

    for(int i = 0; i <= 7; i++){
        for(int j = 0; j <= 7; j++){
            if (board->squares[i][j] != nullptr && board->squares[i][j]->team == team) {
                Piece* piece = board->squares[i][j];
                moves = piece->getValidMoves(board, Square(i, j));

                for (auto it_m = moves->begin(); it_m != moves->end(); ++it_m) {
                    Piece* capt = board->squares[it_m->x][it_m->y];

                    cur_eval = capt != nullptr ? capt->getValue() : 0;
                    cur_eval -= getNextMoveR(Move(Square(i, j), Square(it_m->x, it_m->y)), board, GetOppositeTeam(team), depth);
                    cur_eval -= piece->getType() == PieceType::KING ? 0.2f : 0;
                    cur_eval += (it_m->y - j) * 0.1 * (team == Team::WHITE ? 1 : -1);  
                    cur_eval += (abs(3 - i) - abs(3 - it_m->x)) * 0.1;
                    cur_eval += (abs(3 - it_m->x) < abs(3 - lastx)) ? 0.1 : 0;

                    if (cur_eval > evaluation || dest.x == -1){
                        origin = Square(i, j);
                        dest   = Square(it_m->x, it_m->y);
                        evaluation = cur_eval;
                        lastx = i;
                    }   
                }

                delete moves;
            }
        }
    }
    return Move(origin, dest);
}

///=================================================================
int Engine::getNextMoveR(const Move& move, Board* board, Team team, int depth)
{
    std::vector<Square>* moves = nullptr;
    Piece* captured;

    captured = board->squares[move.d.x][move.d.y];
    board->squares[move.d.x][move.d.y] = board->squares[move.o.x][move.o.y];
    board->squares[move.o.x][move.o.y] = nullptr;

    float evaluation = FLT_MIN;
    float cur_eval;

    for(int i = 0; i <= 7; i++){
        for(int j = 0; j <= 7; j++){
            if (board->squares[i][j] != nullptr && board->squares[i][j]->team == team) {
                Piece* piece = board->squares[i][j];
                moves = piece->getValidMoves(board, Square(i, j));

                for (auto it_m = moves->begin(); it_m != moves->end(); ++it_m) {
                    Piece* capt = board->squares[it_m->x][it_m->y];

                    cur_eval = capt != nullptr ? capt->getValue() : 0;

                    if (depth > 1){
                        cur_eval -= getNextMoveR(Move(Square(i, j), Square(it_m->x, it_m->y)), board, GetOppositeTeam(team), depth - (capt == nullptr ? 2 : 1));
                    }

                    cur_eval -= piece->getType() == PieceType::KING ? 0.2f : 0;

                    if (cur_eval > evaluation)
                        evaluation = cur_eval;
                }

                delete moves;
            }
        }
    }


    board->squares[move.o.x][move.o.y] = board->squares[move.d.x][move.d.y];
    board->squares[move.d.x][move.d.y] = captured;

    return evaluation;
}