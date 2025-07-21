#include "pch.h"

#include "engine.h"
#include <vector>
#include <climits>
#include <algorithm>
#include <float.h>
#include <string>

#include "..\pieces\pawn.h"
#include "..\pieces\rook.h"
#include "..\pieces\bishop.h"
#include "..\pieces\knight.h"
#include "..\pieces\queen.h"
#include "..\pieces\king.h"

std::vector<PieceInfo> wPieces = {
    Pawn(Team::WHITE).getInfo(),
    Rook(Team::WHITE).getInfo(),
    Knight(Team::WHITE).getInfo(),
    Bishop(Team::WHITE).getInfo(),
    Queen(Team::WHITE).getInfo(),
    King(Team::WHITE).getInfo()
};

std::vector<PieceInfo> bPieces = {
    Pawn(Team::BLACK).getInfo(),
    Rook(Team::BLACK).getInfo(),
    Knight(Team::BLACK).getInfo(),
    Bishop(Team::BLACK).getInfo(),
    Queen(Team::BLACK).getInfo(),
    King(Team::BLACK).getInfo()
};

////////////////////////////////////////////////////////////////////
/// Engine
///=================================================================
void Engine::setDepth(int depth)
{
    this->depth = depth;
}

///=================================================================
Move Engine::getNextMove(Game* game)
{
    totalCalls = 0;
    skippedCalls = 0;
    evaluations = 0;
    player = game->getCurrentTurn();
    currentEvaluation = game->getBoard()->getPieceBalance();
    boardString = game->getBoard()->getBoardAsString();

    whitePieceLocations.clear();
    blackPieceLocations.clear();
    cache.clear();

    std::vector<PieceInfo>* pieces[2] = { &wPieces, &bPieces };
    std::vector<Square>* pieceLocations[2] = { &whitePieceLocations, &blackPieceLocations};

    for (int i = 0; i < 2; i++)
    {
        std::vector<std::pair<Square, float>> temp;
        for (const auto& pieceInfo : *pieces[i])
        {
            auto positions = *(game->getBoard()->getPiecePositions(pieceInfo));
            for (const auto& origin : positions)
            {
                const Piece* piece = game->getBoard()->getPiece(Square(origin));
                if (piece != nullptr)
                    temp.emplace_back(origin, piece->getValue());
            }
        }

        std::sort(temp.begin(), temp.end(), [](const auto& a, const auto& b) {
            return a.second > b.second;
            });

        for (const auto& pair : temp)
            (*pieceLocations[i]).push_back(pair.first);
    }

    bestMove = Move();
    getNextMoveR(game, 0, -FLT_MAX, FLT_MAX);

    return bestMove;
}

///=================================================================
float Engine::getPieceEvaluation(const Square& position, const Piece* piece)
{
    int multiplicator = (piece->getInfo().team == Team::WHITE ? 1 : -1);
    float pieceValue = piece->getValue();
    float centerX = 3.5f, centerY = 3.5f;
    float dx = position.getX() - centerX;
    float dy = position.getY() - centerY;
    float distance = std::sqrt(dx * dx + dy * dy);
    float maxDistance = std::sqrt(2 * 3.5f * 3.5f);
    float proximity = 1.0f - (distance / maxDistance);
    float extraValue = 0.2f * proximity * (piece->getInfo().type == PieceType::KING ? -1 : 1);
    
    return multiplicator * (pieceValue + extraValue);
}

///========================== =======================================
float Engine::getNextMoveR(Game* game, int step, float alpha, float beta)
{
    totalCalls += 1;

    if (step == depth)
    {
        evaluations += 1;
        return currentEvaluation * (player == Team::WHITE ? 1 : -1);
    }

    bool maximize = step % 2 == 0;
    const std::vector<Square>& pieceLocations = (game->getCurrentTurn() == Team::WHITE) ? whitePieceLocations : blackPieceLocations;

    for (const auto& origin : pieceLocations)
    {
        if (game->getBoard()->getPiece(origin) == nullptr)
            continue;
    
        const Piece* piece = game->getBoard()->getPiece(origin);

        //////////////////////////////////////////
        auto moves = game->getValidMoves(origin);
        std::vector<std::pair<Square, float>> temp;

        for (const auto& move : moves)
        {
            const Piece* capture = game->getBoard()->getPiece(move);
            if (capture != nullptr)
                temp.emplace_back(move, capture->getValue() * (capture->getInfo().team != game->getCurrentTurn() ? 1 : -1));
            else
                temp.emplace_back(move, 0);
        }

        std::sort(temp.begin(), temp.end(), [](const auto& a, const auto& b) {
            return a.second > b.second;
            });

        std::vector<Square> sortedDestinations;
        for (const auto& pair : temp)
            sortedDestinations.push_back(pair.first);
        //////////////////////////////////////////

        for (const auto& destination : sortedDestinations)
        {
            int pieceIndex = 8 * origin.getX() + origin.getY();
            char pieceChar = getPieceChar(piece->getInfo());
            float pieceValue = getPieceEvaluation(origin, piece);
            float pieceNewValue = getPieceEvaluation(destination, piece);
            float pieceValueDiff = pieceNewValue - pieceValue;

            int captureIndex = 8 * destination.getX() + destination.getY();
            char captureChar = '-';
            float captureValue = 0;

            const Piece* capture = game->getBoard()->getPiece(destination);
            if (capture != nullptr)
            {
                captureValue = getPieceEvaluation(destination, capture);
                captureChar = getPieceChar(capture->getInfo());
            }
            
            if (!game->makeMove(Move(origin, destination)))
                continue;

            currentEvaluation -= captureValue;
            currentEvaluation += pieceValueDiff;
            boardString[pieceIndex] = '-';
            boardString[captureIndex] = pieceChar;

            float curEval;
            std::string key = boardString + (game->getCurrentTurn() == Team::WHITE ? 'w' : 'b') + std::to_string(step);
            if (cache.count(key))
                curEval = cache[key];
            else
                curEval = getNextMoveR(game, step + 1, alpha, beta);

            currentEvaluation += captureValue;
            currentEvaluation -= pieceValueDiff;
            boardString[pieceIndex] = pieceChar;
            boardString[captureIndex] = captureChar;

            cache[key] = curEval;
            game->undoLastMove();

            if (maximize) 
            {
                if (curEval > alpha) 
                {
                    alpha = curEval;
                    if (step == 0)
                        bestMove = Move(origin, destination);
                }
            }
            else 
            {
                if (curEval < beta) 
                {
                    beta = curEval;
                    if (step == 0)
                        bestMove = Move(origin, destination);
                }
            }

            if (alpha >= beta) 
            {
                skippedCalls += 1;
                return maximize ? alpha : beta;
            }
        }
    }

    return maximize ? alpha : beta;
}
