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
enum class Team {
    WHITE = 0,
    BLACK = 1
};

///=================================================================
struct PieceInfo {
    PieceType type = PieceType::PAWN;
    Team team = Team::WHITE;
};

///=================================================================
enum class MoveType {
    IMPOSSIBLE = 0,
    REGULAR = 1,
    PAWN_MOVE = 2,
    PAWN_DOUBLE_MOVE = 3,
    PAWN_CAPTURE = 4,
    PAWN_CAPTURE_EN_PASSANT = 5,
    PAWN_PROMOTION = 6,
    PAWN_CAPTURE_PROMOTION = 7,
    SHORT_CASTLE = 8,
    LONG_CASTLE = 9
};

///=================================================================
typedef std::pair<int, int> Direction;
typedef std::set<Direction> DirectionSet;

////////////////////////////////////////////////////////////////////
/// Generic Functions
///=================================================================
Team getOpponent(Team team);
char getPieceChar(PieceType type);
char getPieceChar(PieceInfo info);

////////////////////////////////////////////////////////////////////
/// Square
///=================================================================
class Square 
{
public:
    Square();
    Square(const std::string& name);
    Square(int px, int py);

    bool operator==(const Square& other) const;
    bool isValid() const;
    int getX() const;
    int getY() const;
    std::string toString() const;

private:
    int x;
    int y;
};

typedef std::vector<Square> SquareVec;

////////////////////////////////////////////////////////////////////
/// Move
///=================================================================
class Move 
{
public:
    Move();
    Move(Square o, Square d);

    bool isValid() const;
    const Square& getOrigin() const;
    const Square& getDestination() const;
    int getDeltaX() const;
    int getDeltaY() const;
    bool sameDiagonal() const;
    bool sameLine() const;
    Move getOpposite() const;
    std::string toString() const;

private:
    Square o;
    Square d;
};

typedef std::vector<Move> MoveVec;

////////////////////////////////////////////////////////////////////
/// Piece
///=================================================================
class Piece
{
public:
    Piece(PieceInfo info);
    virtual std::unique_ptr<Piece> clone() const = 0;

    const PieceInfo& getInfo() const;
    bool doesMoveInDirection(const Direction& direction) const;
    bool doesAttackInDirection(const Direction& direction) const;

    virtual int getValue() const = 0;
    virtual int getMoveRange() const = 0;
    virtual DirectionSet getMoveDirections() const = 0;
    virtual DirectionSet getAttackDirections() const = 0;
    virtual MoveType getMoveType(const Move& move) const = 0;
    virtual SquareVec getMovePath(const Move& move) const = 0;

protected:
    PieceInfo info;
};

///=================================================================
class GenericPiece : public Piece
{
public:
    GenericPiece(PieceInfo info);

protected:
    bool moveInLines;
    bool moveInDiagonals;

    MoveType getGenericMoveType(const Move& move) const;
    SquareVec getGenericMovePath(const Move& move) const;
};

////////////////////////////////////////////////////////////////////
/// Board
///=================================================================
class Board 
{
public:
    Board();

    Board copy() const;
    void clear();
    const Piece* getPiece(const Square& square) const;
    bool movePiece(const Move& move);
    bool putPiece(const Square& square, const Piece& piece);
    bool putPiece(const Square& square, std::unique_ptr<Piece> piece);
    std::unique_ptr<Piece> removePiece(const Square& square);
    const std::set<std::string>* getPiecePositions(const PieceInfo& info) const;
    std::string getBoardAsString() const;

private:
    std::unordered_map<std::string, std::unique_ptr<Piece>> squares;
    std::unordered_map<char, std::set<std::string>> piecePositions;
};

#endif