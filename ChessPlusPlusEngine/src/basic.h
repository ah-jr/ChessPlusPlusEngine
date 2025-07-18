#ifndef BASE_HEADER
#define BASE_HEADER

////////////////////////////////////////////////////////////////////
/// Macros
///=================================================================
#define DECLARE_CLONABLE(ClassName) \
    std::unique_ptr<Piece> clone() const override { \
        return std::make_unique<ClassName>(*this); \
    }

////////////////////////////////////////////////////////////////////
/// Generic Types
///=================================================================
enum class PieceType { 
    PAWN   = 0, 
    BISHOP = 1, 
    KNIGHT = 2, 
    ROOK   = 3, 
    KING   = 4, 
    QUEEN  = 5,
};

///=================================================================
enum class Player {
    WHITE = 0,
    BLACK = 1
};

///=================================================================
enum class MoveType {
    IMPOSSIBLE = 0,
    REGULAR = 1,
    PAWN_MOVE = 2,
    PAWN_DOUBLE_MOVE = 3,
    PAWN_CAPTURE = 4,
    PAWN_PROMOTION = 5,
    PAWN_CAPTURE_PROMOTION = 6,
    SHORT_CASTLE = 7,
    LONG_CASTLE = 8
};

////////////////////////////////////////////////////////////////////
/// Generic Functions
///=================================================================
std::vector<int> getValuesBetween(int a, int b);
std::string PieceTypeToString(PieceType e);
std::string TeamToString(Player player);
Player GetOpponent(Player player);

////////////////////////////////////////////////////////////////////
/// Square
///=================================================================
class Square 
{
private:
    int x;
    int y;

public:
    Square(const std::string& name);
    Square(int px, int py);

    bool operator==(const Square& other) const;
    bool isValid() const;
    int getX() const;
    int getY() const;
    std::string toString() const;
};

typedef std::vector<Square> SquareVec;

///=================================================================
struct MoveInfo {
    MoveType type;
    SquareVec path;
};

typedef std::vector<MoveInfo> MoveInfoVec;

////////////////////////////////////////////////////////////////////
/// Move
///=================================================================
class Move 
{
private:
    Square o;
    Square d;

public:
    Move(Square o, Square d);

    bool isValid() const;
    const Square& getOrigin() const;
    const Square& getDestination() const;
    std::string toString() const;
};

typedef std::vector<Square> MoveVec;

////////////////////////////////////////////////////////////////////
/// Piece
///=================================================================
class Piece
{
protected:
    PieceType type;
    Player team;

public:
    Piece(Player team, PieceType type);
    virtual std::unique_ptr<Piece> clone() const = 0;

    PieceType getType() const;
    Player getTeam() const;

    virtual int getValue() const = 0;
    virtual MoveInfo getMoveInfo(const Move& move) const = 0;
    virtual MoveInfoVec getAllMovesInfo(const Square& origin) const = 0;
};


////////////////////////////////////////////////////////////////////
/// Board
///=================================================================
class Board 
{
private:
    std::unordered_map<std::string, std::unique_ptr<Piece>> squares;

public:
    Board();

    void clear();
    const Piece* getPiece(const Square& square) const;
    bool movePiece(const Move& move);
    bool putPiece(const Square& square, const Piece& piece);
    bool clearSquare(const Square& square);
};

#endif