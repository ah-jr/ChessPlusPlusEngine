#ifndef BASE_HEADER
#define BASE_HEADER

////////////////////////////////////////////////////////////////////
/// Generic Types
///=================================================================
class Piece;

///=================================================================
enum class PieceType { 
    PAWN = 0, 
    BISHOP = 1, 
    KNIGHT = 2, 
    ROOK = 3, 
    KING = 4, 
    QUEEN = 5,
};

///=================================================================
enum class Team { 
    WHITE = 0,
    BLACK = 1
};

///=================================================================
typedef struct Square {
    Square() {}
    Square(int x, int y) : x(x), y(y) {}
    int x;
    int y; 
} Square;

///=================================================================
typedef struct Board { 
    Piece* squares[8][8] = {nullptr};

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

    public:
        Team team;
        
        Piece(Team team, PieceType type); 
        virtual bool checkValidMove(Square move, Board* board) = 0;
        PieceType getType();
};

#endif