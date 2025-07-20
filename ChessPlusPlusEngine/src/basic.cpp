#include "pch.h"

#include "basic.h"

////////////////////////////////////////////////////////////////////
/// Generic Functions
///=================================================================
Team getOpponent(Team team)
{
    return team == Team::WHITE ? Team::BLACK : Team::WHITE;
}

///=================================================================
std::string teamToString(Team e)
{
    switch (e)
    {
    case Team::WHITE: return "White";
    case Team::BLACK: return "Black";
    default:          return "Unknown";
    }
}

///=================================================================
std::string  pieceTypeToString(PieceType type)
{
    switch (type)
    {
    case PieceType::PAWN:   return "Pawn";
    case PieceType::BISHOP: return "Bishop";
    case PieceType::KNIGHT: return "Knight";
    case PieceType::ROOK:   return "Rook";
    case PieceType::KING:   return "King";
    case PieceType::QUEEN:  return "Queen";
    default:                return "Unknown";
    }
}

///=================================================================
std::string  pieceInfoToString(PieceInfo info)
{
    return teamToString(info.team) + pieceTypeToString(info.type);
}

////////////////////////////////////////////////////////////////////
/// Square
///=================================================================
Square::Square()
    : x(-1), y(-1)
{
}

///=================================================================
Square::Square(const std::string& name)
    : x(-1), y(-1)
{
    if (name.size() != 2)
        return;

    int px = name[0] - 'a';
    int py = name[1] - '1';

    *this = Square(px, py);
}

///=================================================================
Square::Square(int px, int py)
    : x(-1), y(-1)
{
    if (px < 0 || px > 7 || py < 0 || py > 7)
        return;

    x = px;
    y = py;
}

///=================================================================
bool Square::operator==(const Square& other) const 
{
    return (x == other.x && y == other.y);
}

///=================================================================
bool Square::isValid() const
{
    return x >= 0 && x <= 7 && y >= 0 && y <= 7;
}

///=================================================================
int Square::getX() const
{
    return x;
}

///=================================================================
int Square::getY() const
{
    return y;
}

///=================================================================
std::string Square::toString() const
{
    if (!isValid())
        return "Invalid";

    return { char('a' + x), char('1' + y) };
}

////////////////////////////////////////////////////////////////////
/// Move
///=================================================================
Move::Move()
    : o(Square()), d(Square())
{
}

///=================================================================
Move::Move(Square o, Square d)
    : o(o), d(d)
{
}

///=================================================================
std::string Move::toString() const
{
    return o.toString() + d.toString();
}

///=================================================================
bool Move::isValid() const
{
    return o.isValid() && d.isValid() && !(o == d);
}

///=================================================================
const Square& Move::getOrigin() const
{
    return o;
}

///=================================================================
const Square& Move::getDestination() const
{
    return d;
}

///=================================================================
int Move::getDeltaX() const
{
    return d.getX() - o.getX();
}


///=================================================================
int Move::getDeltaY() const
{
    return d.getY() - o.getY();
}

///=================================================================
bool Move::sameDiagonal() const
{
    return std::abs(getDeltaX()) == std::abs(getDeltaY());
}

///=================================================================
bool Move::sameLine() const
{
    return (getDeltaX() == 0 || getDeltaY() == 0);
}

///=================================================================
Move Move::getOpposite() const
{
    return Move(d, o);
}

////////////////////////////////////////////////////////////////////
/// Piece
///=================================================================
Piece::Piece(PieceInfo info)
    : info(info)
{
}

///=================================================================
const PieceInfo& Piece::getInfo() const
{
    return this->info;
}

///=================================================================
bool Piece::doesMoveInDirection(const Direction& direction) const
{
    auto directions = getMoveDirections();
    if (directions.find(direction) != directions.end())
        return true;

    return false;
}

///=================================================================
bool Piece::doesAttackInDirection(const Direction& direction) const
{
    auto directions = getAttackDirections();
    if (directions.find(direction) != directions.end())
        return true;

    return false;
}

///=================================================================
GenericPiece::GenericPiece(PieceInfo info)
    : Piece(info)
    , moveInLines(false)
    , moveInDiagonals(false)
{
}

///=================================================================
MoveType GenericPiece::getGenericMoveType(const Move& move) const
{
    if (!move.isValid())
        return MoveType::IMPOSSIBLE;

    if (!((moveInLines && move.sameLine()) || (moveInDiagonals && move.sameDiagonal())))
        return MoveType::IMPOSSIBLE;

    return MoveType::REGULAR;
}

///=================================================================
SquareVec GenericPiece::getGenericMovePath(const Move& move) const
{
    SquareVec result;

    if (getMoveType(move) == MoveType::IMPOSSIBLE)
        return result;

    int xStep = (move.getDeltaX() == 0) ? 0 : (move.getDeltaX() > 0 ? 1 : -1);
    int yStep = (move.getDeltaY() == 0) ? 0 : (move.getDeltaY() > 0 ? 1 : -1);
    int steps = std::max(std::abs(move.getDeltaX()), std::abs(move.getDeltaY()));

    for (int i = 1; i <= steps; ++i)
        result.emplace_back(move.getOrigin().getX() + i * xStep, move.getOrigin().getY() + i * yStep);

    return result;
}

////////////////////////////////////////////////////////////////////
/// Board
///=================================================================
Board::Board()
{
    clear();
}

///=================================================================
void Board::clear()
{
    for (int x = 0; x < 8; x++)
        for (int y = 0; y < 8; y++)
            squares[Square(x, y).toString()] = nullptr;

    piecePositions.clear();
}

///=================================================================
const Piece* Board::getPiece(const Square& square) const
{
    if (!square.isValid())
        return nullptr;
    
    auto it = squares.find(square.toString());
    return it != squares.end() ? it->second.get() : nullptr;
}

///=================================================================
bool Board::movePiece(const Move& move)
{
    if (!move.isValid())
        return false;

    const Piece* piece = getPiece(move.getOrigin());
    if (piece == nullptr)
        return false;

    const Piece* capture = getPiece(move.getDestination());
    if (capture != nullptr)
        piecePositions[pieceInfoToString(capture->getInfo())].erase(move.getDestination().toString());

    piecePositions[pieceInfoToString(piece->getInfo())].erase(move.getOrigin().toString());
    piecePositions[pieceInfoToString(piece->getInfo())].insert(move.getDestination().toString());
    squares[move.getDestination().toString()] = std::move(squares[move.getOrigin().toString()]);
    return true;
}

///=================================================================
bool Board::putPiece(const Square& square, const Piece& piece)
{
    if (!square.isValid())
        return false;

    piecePositions[pieceInfoToString(piece.getInfo())].insert(square.toString());
    squares[square.toString()] = piece.clone();
    return true;
}

///=================================================================
bool Board::insertPiece(const Square& square, std::unique_ptr<Piece> piece)
{
    if (!square.isValid())
        return false;

    piecePositions[pieceInfoToString(piece->getInfo())].insert(square.toString());
    squares[square.toString()] = std::move(piece);
    return true;
}

///=================================================================
std::unique_ptr<Piece> Board::removePiece(const Square& square)
{
    if (!square.isValid())
        return nullptr;

    const Piece* piece = getPiece(square);
    if (piece != nullptr)
    {
        piecePositions[pieceInfoToString(piece->getInfo())].erase(square.toString());
        return std::move(squares[square.toString()]);
    }

    return nullptr;
}

///=================================================================
const std::set<std::string>* Board::getPiecePositions(const PieceInfo& info)
{
    auto it = piecePositions.find(pieceInfoToString(info));
    return it != piecePositions.end() ? &it->second : nullptr;
}
