#ifndef BASE_HEADER
#define BASE_HEADER

#include <vector>

////////////////////////////////////////////////////////////////////
/// Generic Types
///=================================================================
class Piece;

///=================================================================
enum class PieceType { 
    PAWN   = 0, 
    BISHOP = 1, 
    KNIGHT = 2, 
    ROOK   = 3, 
    KING   = 4, 
    QUEEN  = 5,
};

constexpr const char* PieceTypeToString(PieceType e)
{
    switch (e)
    {
        case PieceType::PAWN:   return "Pawn";
        case PieceType::BISHOP: return "Bishop";
        case PieceType::KNIGHT: return "Knight";
        case PieceType::ROOK:   return "Rook";
        case PieceType::KING:   return "King";
        case PieceType::QUEEN:  return "Queen";
    }
}

///=================================================================
enum class Team { 
    WHITE = 0,
    BLACK = 1
};

constexpr const char* TeamToString(Team e)
{
    switch (e)
    {
        case Team::WHITE: return "White";
        case Team::BLACK: return "Black";
    }
}

Team GetOppositeTeam(Team team);

///=================================================================
typedef struct Square {
    Square() {}
    Square(int x, int y) : x(x), y(y) {}
    int x;
    int y; 
} Square;

///=================================================================
typedef struct Move {
    Move() {}
    Move(Square o, Square d) : o(o), d(d) {}
    Square o;
    Square d; 
} Move;

///=================================================================
typedef struct Board { 
    Piece* squares[8][8] = {nullptr};

    bool blackCastleL = true;
    bool blackCastleR = true;
    bool whiteCastleL = true;
    bool whiteCastleR = true;

    Square checkIfContainsPiece(Piece* piece){
        for (int i=0; i<8; i++)
            for (int j=0; j<8; j++)
                if (squares[i][j] == piece)
                    return Square(i, j);
        return Square(-1, -1);
    }

    bool checkIfOutOfBounds(Square square){
        return (square.x < 0 || square.x > 7 || square.y < 0 || square.y > 7);
    }
} Board;

////////////////////////////////////////////////////////////////////
/// Piece
///=================================================================
class Piece
{ 
    private:
        PieceType type;

    protected:
        bool checkValidSquare(Square square);

    public:
        Team team;
        
        Piece(Team team, PieceType type); 
        virtual const int getValue() = 0;
        virtual bool checkValidMove(Square move, Board* board) = 0;
        virtual std::vector<Square>* getValidMoves(Board* board, Square pos) = 0;
        PieceType getType();
};

#endif