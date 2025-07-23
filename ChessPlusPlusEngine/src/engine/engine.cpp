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
    this->depth = depth * 2;
}

///=================================================================
MoveVec Engine::getValidMoves(const Square& origin)
{
    MoveVec result;
    if (!origin.isValid())
        return result;

    const Piece* piece = board.getPiece(origin);
    if (piece == nullptr)
        return result;

    DirectionSet attacks = piece->getAttackDirections();
    DirectionSet moves = piece->getMoveDirections();

    std::vector<std::pair<const DirectionSet&, bool>> allDirections = {
        {attacks, true},
        {moves, false}
    };

    for (auto& dir : allDirections)
    {
        const DirectionSet& directions = dir.first;
        bool isAttack = dir.second;

        for (const Direction& dir : directions)
        {
            int dx = dir.first;
            int dy = dir.second;

            int x = origin.getX();
            int y = origin.getY();

            int range = piece->getMoveRange();
            if (piece->getInfo().type == PieceType::PAWN && !isAttack)
            {
                if (piece->getInfo().team == Team::WHITE && y == 1)
                    range++;

                if (piece->getInfo().team == Team::BLACK && y == 6)
                    range++;
            }

            for (int j = 0; j < range; j++)
            {
                x += dx;
                y += dy;

                Square dest(x, y);
                if (!dest.isValid())
                    break;

                const Piece* target = board.getPiece(dest);
                if (isAttack)
                {
                    if (target != nullptr)
                    {
                        if (target->getInfo().team != piece->getInfo().team)
                            result.push_back(Move(origin, dest));
                        break;
                    }
                }
                else
                {
                    if (target != nullptr)
                        break;
                    result.push_back(Move(origin, dest));
                }
            }
        }
    }

    return result;
}

///=================================================================
Move Engine::getNextMove(Game* referenceGame)
{
    totalCalls = 0;
    skippedCalls = 0;
    evaluations = 0;
    
    board = referenceGame->getBoard()->copy();
    playerTeam = stepTeam = referenceGame->getCurrentTurn();
    currentEvaluation = initialEvaluation = referenceGame->getPieceBalance();

    boardString = board.getBoardAsString();

    whitePieceLocations.clear();
    blackPieceLocations.clear();
    cache.clear();

    std::vector<PieceInfo>* pieces[2] = { &wPieces, &bPieces };
    std::unordered_set<std::string>* pieceLocations[2] = { &whitePieceLocations, &blackPieceLocations};

    for (int i = 0; i < 2; i++)
    {
        std::vector<std::pair<std::string, float>> temp;
        for (const auto& pieceInfo : *pieces[i])
        {
            auto positions = *(board.getPiecePositions(pieceInfo));
            for (const auto& origin : positions)
            {
                const Piece* piece = board.getPiece(Square(origin));
                if (piece != nullptr)
                    temp.emplace_back(origin, piece->getValue());
            }
        }

        std::sort(temp.begin(), temp.end(), [](const auto& a, const auto& b) {
            return a.second < b.second;
            });

        for (const auto& pair : temp)
            (*pieceLocations[i]).insert(pair.first);
    }

    bestMove = Move();
    getNextMoveR(0, -FLT_MAX, FLT_MAX, true);

    return bestMove;
}

///=================================================================
float Engine::getPieceEvaluation(const Square& position, const Piece* piece)
{
    bool isWhite = piece->getInfo().team == Team::WHITE;

    int sign = (isWhite ? 1 : -1);
    float pieceValue = piece->getValue();
    float centerX = 3.5f, centerY = 3.5f;
    float dx = position.getX() - centerX;
    float dy = position.getY() - centerY;
    float distance = std::sqrt(dx * dx + dy * dy);
    float maxDistance = std::sqrt(2 * 3.5f * 3.5f);
    float proximity = 1.0f - (distance / maxDistance);
    float bonus = 0.1f * proximity;

    switch (piece->getInfo().type)
    {
    case PieceType::KING:
        bonus *= -5.f;
        break;

    case PieceType::QUEEN:
        bonus *= -1.f;
        break;

    case PieceType::PAWN:
        if (isWhite && position.getY() >= 4)
            bonus += 0.5f * (position.getY() / 7.f);
        else if (position.getY() <= 3)
            bonus += 0.5f * ((7 - position.getY()) / 7.f);

        break;

    default:

        if ((isWhite && position.getY() >= 4) || position.getY() < 3)
        {
            auto positions = board.getPiecePositions({ PieceType::KING, getOpponent(playerTeam) });
            if (positions->empty())
                return false;

            Square kingPosition(*positions->begin());

            float dx = position.getX() - kingPosition.getX();
            float dy = position.getY() - kingPosition.getY();
            float distanceToKing = std::sqrt(dx * dx + dy * dy);

            bonus += 0.1f * (1.f - (distanceToKing / 10.f));
        }

        break;
    }

    return sign * (pieceValue + bonus);
}

///========================== =======================================
float Engine::getNextMoveR(int step, float alpha, float beta, bool maximize)
{
    totalCalls += 1;

    if (step >= depth)
    {
        evaluations += 1;
        return currentEvaluation * (playerTeam == Team::WHITE ? 1 : -1);
    }

    std::unordered_set<std::string>& pieceLocations = (stepTeam == Team::WHITE) ? whitePieceLocations : blackPieceLocations;
    std::unordered_set<std::string>& opponentLocations = (stepTeam == Team::BLACK) ? whitePieceLocations : blackPieceLocations;

    MoveVec allMoves = {};

    for (const auto& origin : pieceLocations)
    {
        if (board.getPiece(Square(origin)) == nullptr)
            continue;

        MoveVec moves = getValidMoves(Square(origin));
        allMoves.reserve(allMoves.size() + moves.size());
        allMoves.insert(allMoves.end(),
            std::make_move_iterator(moves.begin()),
            std::make_move_iterator(moves.end()));
    }

    std::sort(allMoves.begin(), allMoves.end(), [&](const Move& a, const Move& b) {
        const Piece* captA = board.getPiece(a.getDestination());
        const Piece* captB = board.getPiece(b.getDestination());

        const bool isCaptA = captA != nullptr;
        const bool isCaptB = captB != nullptr;

        if (isCaptA != isCaptB)
            return isCaptA > isCaptB;

        if (isCaptA)
            return captA->getValue() > captB->getValue();

        const Piece* pieceA = board.getPiece(a.getOrigin());
        const Piece* pieceB = board.getPiece(b.getOrigin());

        return pieceA->getValue() < pieceB->getValue();
    });

    for (const auto& move : allMoves)
    {
        const auto& origin = move.getOrigin();
        const auto& destination = move.getDestination();

        const Piece* piece = board.getPiece(origin);

        int pieceIndex = 8 * origin.getX() + origin.getY();
        char pieceChar = getPieceChar(piece->getInfo());
        float pieceValue = getPieceEvaluation(origin, piece);
        float pieceNewValue = getPieceEvaluation(destination, piece);
        float pieceValueDiff = pieceNewValue - pieceValue;

        int captureIndex = 8 * destination.getX() + destination.getY();
        char captureChar = '-';
        float captureValue = 0;
        int refStep = step;

        std::unique_ptr<Piece> capture = board.removePiece(destination);
        if (capture != nullptr)
        {
            captureValue = getPieceEvaluation(destination, capture.get());
            captureChar = getPieceChar(capture->getInfo());
        }
        else 
        {
            if ((depth - refStep) > 2)
            {
                refStep += (depth - refStep) / 2;
                if (refStep % 2 != step % 2)
                    refStep += 1;
            }
        }

        float oldEvaluation = currentEvaluation;
        float newEvaluation = currentEvaluation + (pieceValueDiff - captureValue);

        if ((stepTeam == Team::WHITE ? 1 : -1) * (newEvaluation - initialEvaluation) < 0.5f)
        {
            if ((depth - refStep) > 2)
            {
                refStep += (depth - refStep) / 2;
                if (refStep % 2 != step % 2)
                    refStep += 1;
            }
        }

        currentEvaluation = newEvaluation;
        boardString[pieceIndex] = '-';
        boardString[captureIndex] = pieceChar;

        opponentLocations.erase(destination.toString());
        pieceLocations.erase(origin.toString());
        pieceLocations.insert(destination.toString());

        board.movePiece(Move(origin, destination));

        float curEval;
        std::string key = boardString + std::to_string(refStep);
        if (cache.count(key))
            curEval = cache[key];
        else
        {
            stepTeam = getOpponent(stepTeam);
            curEval = getNextMoveR(refStep + 1, alpha, beta, !maximize);
            stepTeam = getOpponent(stepTeam);
        }

        pieceLocations.insert(origin.toString());
        pieceLocations.erase(destination.toString());
        if (capture != nullptr)
        {
            opponentLocations.insert(destination.toString());
        }

        currentEvaluation = oldEvaluation;
        boardString[pieceIndex] = pieceChar;
        boardString[captureIndex] = captureChar;

        cache[key] = curEval;

        board.movePiece(Move(destination, origin));
        if (capture != nullptr)
            board.putPiece(destination, std::move(capture));

        if (maximize && curEval > alpha)
        {
            alpha = curEval;
            if (step == 0)
                bestMove = Move(origin, destination);
        }
        else if (!maximize && curEval < beta)
            beta = curEval;

        if (alpha >= beta)
        {
            skippedCalls += 1;
            return maximize ? alpha : beta;
        }

    }

    return maximize ? alpha : beta;
}
