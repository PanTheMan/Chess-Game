//
//  position.hpp
//  PawnPusher9000
//
//  Created by Lavi on 2015-11-22.
//  Copyright © 2015 Lavi. All rights reserved.
//

#ifndef position_hpp
#define position_hpp
#include <string>
#include <map>
#include <vector>
using namespace std;

enum PositionFlag{
    NONE,
    CHECK,
    CHECKMATE,
    STALEMATE,
    FIFTYMOVE
};

enum Colour{
    WHITE,
    BLACK
};

struct MoveFootprint{
    int begpos;
    int endpos;
    char pieceOnEndPos;
    int canEnPassantAt;
    bool castleAllowed[4];
    bool isPromotion;
    int plySinceLastAction;
};

class ChessPosition{
    char ** square;
    bool isBlackToMove;
    //int canEnPassantAt;
    bool castleAllowed[4]; //whether K/Q-side castling is still available for white/black
    
    map<char, vector<int> > pieceMem;
public:
    int canEnPassantAt;
    int plySinceLastAction; //Number of half moves since last piece capture or pawn push (for enforcing 50  move rule)
    int fullMoves;
    ChessPosition(string FEN="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    ChessPosition(const ChessPosition & toClone);
    ~ChessPosition();
    void clearBoard();
    void setupPosition(string FEN="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    string getFEN() const;
    //bool isPsuedoLegalMove(string begPos, string endPos);
    bool isPsuedoLegalMove(int begPos, int endPos);
    bool isLegalMove(int begPos, int endPos);
    bool isLegalMove(string begPos, string endPos){
        return isLegalMove(ChessSquareConverts(begPos), ChessSquareConverts(endPos));
    }
    char getSquare(int sqr) const;
    char getSquare(string sqrName) const;
    void setSquare(int sqr, char piece);
    void setSquare(string sqrName, char piece);
    MoveFootprint & makeMove(int begPos, int endPos, char promotion=' '); //returns a footprint that can be used to undo
    MoveFootprint & makeMove(string begPos, string endPos, char promotion=' '){
        return makeMove( ChessSquareConverts(begPos), ChessSquareConverts(endPos), promotion);
    }
    void undoMove(MoveFootprint & footprint);
    bool kingCanBeCaptured();
    bool isCheck();
    PositionFlag flag(); //returns NONE, CHECK, ...
    bool canCastle(Colour colour, bool kingSide) const;
    Colour colourToMove() const{ //0 for white, 1 for black
        return ((Colour)isBlackToMove);
    }
    void setColourToMove(bool isBlack){
        isBlackToMove=isBlack;
    }
    void toggleColour(){
        isBlackToMove=!isBlackToMove;
    }
    bool isLegalPosition(); // should have a few sanity checks such as 1 of each king, etc..
    bool isValidSquare(int sqr){
        return sqr>=0 && sqr<64;
    }
    /*bool isValidSquare(string sqrName){
        return (sqrName[0]>='a' && sqrName[0]<='h' && sqrName[1]>='1' && sqrName[1]<='8');
    }*/
    int numPiecesOfType(char piece);
    int getSquareOfPiece(char piece, int instanceNum);
    vector<int> getLegalMovesFrom(int begPos, bool pseudo=false);
    bool anyLegalMoves();
    const string pieceTypes; //={'K','Q','R'..};
    vector<int> getAllLegalMoves();
    vector<int> getLegalMovesTo(int endPos, bool pseudo=false);
    
    static string ChessSquareConverti(int n){
        string sqrn="  ";
        sqrn[0]=n%8+'a';
        sqrn[1]=n/8+'1';
        return sqrn;
    }
    
    static int ChessSquareConverts(string s){
        int n=s[1]-'1';
        n*=8;
        n+=s[0]-'a';
        return n;
    }
    static Colour colourOf(char piece){
        if ('A'<=piece && piece<='Z') return WHITE;
        else return BLACK;
    }
};




/* Not used anymore
struct ChessPly{
    ChessPosition & pos;
    ChessPly * prev;
};
*/



#endif /* position_hpp */
