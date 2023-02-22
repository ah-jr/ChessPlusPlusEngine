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
    SquareVec moves = nullptr;
    SquareVec wPieces = new std::vector<Square>();
    SquareVec bPieces = new std::vector<Square>();
    SquareVec sPieces = (team == Team::WHITE) ? wPieces : bPieces;

    for(int i = 0; i <= 7; i++)
        for(int j = 0; j <= 7; j++)
            if (board->squares[i][j] != nullptr)
                if (board->squares[i][j]->team == Team::WHITE)
                    wPieces->push_back(Square(i, j));
                else    
                    bPieces->push_back(Square(i, j));

    Square origin = Square(-1, -1);
    Square dest = Square(-1, -1);
    float evaluation = FLT_MIN;
    float cur_eval;
    int lastx = 0;

    for (auto it_p = sPieces->begin(); it_p != sPieces->end(); ++it_p) {
        Piece* piece = board->squares[it_p->x][it_p->y];
        moves = piece->getValidMoves(board, *it_p);

        for (auto it_m = moves->begin(); it_m != moves->end(); ++it_m) {
            Piece* capt = board->squares[it_m->x][it_m->y];

            cur_eval = capt != nullptr ? capt->getValue() : 0;

            Square temp = *it_p;
            *it_p = *it_m;
            cur_eval -= getNextMoveR(Move(temp, *it_m), board, wPieces, bPieces, GetOppositeTeam(team), depth);
            *it_p = temp;

            cur_eval -= piece->getType()                  == PieceType::KING ? 0.2f : 0;
            cur_eval += (it_m->y - it_p->y) * 0.1 * (team == Team::WHITE ? 1 : -1);  
            cur_eval += (abs(3 - it_p->x) - abs(3 - it_m->x)) * 0.1;
            cur_eval += (abs(3 - lastx) -   abs(3 - it_m->x)) * 0.1;

            if (cur_eval > evaluation || dest.x == -1){
                origin = * it_p;
                dest   = Square(it_m->x, it_m->y);
                evaluation = cur_eval;
                lastx = it_p->x;
            }   
        }

        delete moves;
    }

    delete wPieces, bPieces;
    return Move(origin, dest);
}

///=================================================================
int Engine::getNextMoveR(const Move& move, Board* board, SquareVec wPieces, SquareVec bPieces, Team team, int depth)
{
    SquareVec moves = nullptr;
    SquareVec sPieces = (team == Team::WHITE) ? wPieces : bPieces;

    Piece* captured;

    captured = board->squares[move.d.x][move.d.y];
    board->squares[move.d.x][move.d.y] = board->squares[move.o.x][move.o.y];
    board->squares[move.o.x][move.o.y] = nullptr;

    float evaluation = FLT_MIN;
    float cur_eval;
    bool reinsert = false;
    Square old;
    std::vector<Square>::iterator ins_pos;

    for (auto it_p = sPieces->begin(); it_p != sPieces->end(); ++it_p) {
        if (*it_p == move.d){
            old = *it_p;
            sPieces->erase(it_p);
            ins_pos = it_p;
            reinsert = true;
            break;
        }
    }

    for (auto it_p = sPieces->begin(); it_p != sPieces->end(); ++it_p) {
        Piece* piece = board->squares[it_p->x][it_p->y];
        moves = piece->getValidMoves(board, *it_p);

        for (auto it_m = moves->begin(); it_m != moves->end(); ++it_m) {
            Piece* capt = board->squares[it_m->x][it_m->y];

            cur_eval = capt != nullptr ? capt->getValue() : 0;

            if (depth > 1){
                Square temp = *it_p;
                *it_p = *it_m;
                int realDepth = depth - (capt == nullptr ? 2 : 1) - (evaluation > cur_eval ? 1 : 0);

                cur_eval -= getNextMoveR(Move(temp, *it_m), board, wPieces, bPieces, GetOppositeTeam(team), realDepth);
                *it_p = temp;
            }

            cur_eval -= piece->getType() == PieceType::KING ? 0.2f : 0;

            if (cur_eval > evaluation)
                evaluation = cur_eval;
        }

        delete moves;

        if (evaluation > 2) 
            break;
    }

    if (reinsert){
        sPieces->insert(ins_pos, old);
    }

    board->squares[move.o.x][move.o.y] = board->squares[move.d.x][move.d.y];
    board->squares[move.d.x][move.d.y] = captured;

    return evaluation;
}