#include "pch.h"

#include "engine.h"
#include <vector>
#include <climits>
#include <algorithm>
#include <float.h>

////////////////////////////////////////////////////////////////////
/// Engine
///=================================================================
Move Engine::getNextMove(Game* game, Board* board, Team team, int depth)
{
    SquareVec moves;
    SquareVec wPieces;
    SquareVec bPieces;
    SquareVec* sPieces = (team == Team::WHITE) ? &wPieces : &bPieces;

    for(int i = 0; i <= 7; i++)
        for (int j = 0; j <= 7; j++) 
        {
            const Piece* piece = board->getPiece(Square(i, j));
            if (piece != nullptr)
                if (piece->getInfo().team == Team::WHITE)
                    wPieces.push_back(Square(i, j));
                else
                    bPieces.push_back(Square(i, j));
        }

    Square origin = Square(-1, -1);
    Square dest = Square(-1, -1);
    float evaluation = FLT_MIN;
    float cur_eval;
    int lastx = 0;

    for (auto it_p = sPieces->begin(); it_p != sPieces->end(); ++it_p) {
        const Piece* piece = board->getPiece(*it_p);
        moves = game->getValidMoves(*it_p);

        for (auto it_m = moves.begin(); it_m != moves.end(); ++it_m) {
            const Piece* capt = board->getPiece(*it_m);

            cur_eval = capt != nullptr ? capt->getValue() : 0;

            Square temp = *it_p;
            *it_p = *it_m;
            cur_eval -= getNextMoveR(game, Move(temp, *it_m), board, &wPieces, &bPieces, getOpponent(team), depth);
            *it_p = temp;

            cur_eval -= piece->getInfo().type == PieceType::KING ? 0.2f : 0;
            cur_eval += (it_m->getY() - it_p->getY()) * 0.1 * (team == Team::WHITE ? 1 : -1);
            cur_eval += (abs(3 - it_p->getX()) - abs(3 - it_m->getX())) * 0.1;
            cur_eval += (abs(3 - lastx) -   abs(3 - it_m->getX())) * 0.1;

            if (cur_eval > evaluation || dest.getX() == -1){
                origin = * it_p;
                dest   = Square(it_m->getX(), it_m->getY());
                evaluation = cur_eval;
                lastx = it_p->getX();
            }   
        }
    }

    return Move(origin, dest);
}

///=================================================================
int Engine::getNextMoveR(Game* game, const Move& move, Board* board, SquareVec* wPieces, SquareVec* bPieces, Team team, int depth)
{
    SquareVec moves;
    SquareVec* sPieces = (team == Team::WHITE) ? wPieces : bPieces;

    std::unique_ptr<Piece> captured = board->getPiece(move.getOrigin())->clone();
    board->movePiece(move);

    float evaluation = FLT_MIN;
    float cur_eval;
    bool reinsert = false;
    Square old(-1,-1);
    //std::vector<Square>::iterator ins_pos;

    //for (auto it_p = sPieces->begin(); it_p != sPieces->end(); ++it_p) {
    //    if (*it_p == move.getDestination()){
    //        old = *it_p;
    //        sPieces->erase(it_p);
    //        ins_pos = it_p;
    //        reinsert = true;
    //        break;
    //    }
    //}

    int index = -1;
    for (int i = 0; i < sPieces->size(); ++i) {
        if ((*sPieces)[i] == move.getDestination()) {
            old = (*sPieces)[i];
            sPieces->erase(sPieces->begin() + i);
            index = i;
            reinsert = true;
            break;
        }
    }

    for (auto it_p = sPieces->begin(); it_p != sPieces->end(); ++it_p) {
        const Piece* piece = board->getPiece(*it_p);
        moves = game->getValidMoves(*it_p);

        for (auto it_m = moves.begin(); it_m != moves.end(); ++it_m) {
            const Piece* capt = board->getPiece(*it_m);

            cur_eval = capt != nullptr ? capt->getValue() : 0;

            if (depth > 1){
                Square temp = *it_p;
                *it_p = *it_m;
                int realDepth = depth - (capt == nullptr ? 2 : 1) - (evaluation > cur_eval ? 1 : 0);

                cur_eval -= getNextMoveR(game, Move(temp, *it_m), board, wPieces, bPieces, getOpponent(team), realDepth);
                *it_p = temp;
            }

            cur_eval -= piece->getInfo().type == PieceType::KING ? 0.2f : 0;

            if (cur_eval > evaluation)
                evaluation = cur_eval;
        }

        if (evaluation > 2) 
            break;
    }

    //if (reinsert){
    //    sPieces->insert(ins_pos, old);
    //}

    if (reinsert) {
        sPieces->insert(sPieces->begin() + index, old);
    }

    Move inverseMove = Move(move.getDestination(), move.getOrigin());

    board->movePiece(inverseMove);
    board->putPiece(move.getDestination(), *captured);

    return evaluation;
}