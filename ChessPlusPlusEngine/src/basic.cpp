#include "pch.h"

#include "basic.h"

////////////////////////////////////////////////////////////////////
/// Generic Functions
///=================================================================
std::vector<int> getValuesBetween(int a, int b)
{
    std::vector<int> result;

    if (a == b)
        return result;

    const int increment = (a > b) ? -1 : 1; 

    for (int i = a; i != b; i += increment)
        result.push_back(i + increment);
    
    return result;
}

///=================================================================
Player GetOpponent(Player player)
{
    return player == Player::WHITE ? Player::BLACK : Player::WHITE;
}

///=================================================================
std::string  PieceTypeToString(PieceType e)
{
    switch (e)
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
std::string TeamToString(Player e)
{
    switch (e)
    {
    case Player::WHITE: return "White";
    case Player::BLACK: return "Black";
    default:            return "Unknown";
    }
}

////////////////////////////////////////////////////////////////////
/// Square
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

////////////////////////////////////////////////////////////////////
/// Piece
///=================================================================
Piece::Piece(Player team, PieceType type)
{
    this->team = team;
    this->type = type;
}

///=================================================================
PieceType Piece::getType() const
{
    return this->type;
}

///=================================================================
Player Piece::getTeam() const
{
    return this->team;
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

    if (getPiece(move.getOrigin()) == nullptr)
        return false;

    squares[move.getDestination().toString()] = std::move(squares[move.getOrigin().toString()]);
    return true;
}

///=================================================================
bool Board::putPiece(const Square& square, const Piece& piece)
{
    if (!square.isValid())
        return false;

    squares[square.toString()] = piece.clone();
    return true;
}

///=================================================================
bool Board::clearSquare(const Square& square)
{
    if (!square.isValid())
        return false;

    squares[square.toString()] = nullptr;
    return true;
}
