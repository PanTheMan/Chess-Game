
//
//  computer5.cpp
//  PawnPusher9000
//
//  Created by Lavi on 2015-12-02.
//  Copyright © 2015 Lavi. All rights reserved.
//

#include "computer5.h"
#include <ctype.h>
#include <cstdlib>
#include <time.h>
#include <iostream>
#include <algorithm>
#include <queue>

//I am using Tomasz Michniewski's evaluation heuristic https://chessprogramming.wikispaces.com/Simplified+evaluation+function

long total_eval=0;

const float PawnSquare[64]={
    0,  0,  0,  0,  0,  0,  0,  0,
    50, 50, 50, 50, 50, 50, 50, 50,
    10, 10, 20, 30, 30, 20, 10, 10,
    5,  5, 10, 25, 25, 10,  5,  5,
    0,  0,  0, 20, 20,  0,  0,  0,
    5, -5,-10,  0,  0,-10, -5,  5,
    5, 10, 10,-20,-20, 10, 10,  5,
    0,  0,  0,  0,  0,  0,  0,  0
};

const float KnightSquare[64]={
    -50,-40,-30,-30,-30,-30,-40,-50,
    -40,-20,  0,  0,  0,  0,-20,-40,
    -30,  0, 10, 15, 15, 10,  0,-30,
    -30,  5, 15, 20, 20, 15,  5,-30,
    -30,  0, 15, 20, 20, 15,  0,-30,
    -30,  5, 10, 15, 15, 10,  5,-30,
    -40,-20,  0,  5,  5,  0,-20,-40,
    -50,-40,-30,-30,-30,-30,-40,-50
};

const float BishopSquare[64]={
    -20,-10,-10,-10,-10,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  5,  5, 10, 10,  5,  5,-10,
    -10,  0, 10, 10, 10, 10,  0,-10,
    -10, 10, 10, 10, 10, 10, 10,-10,
    -10,  5,  0,  0,  0,  0,  5,-10,
    -20,-10,-10,-10,-10,-10,-10,-20
};

const float RookSquare[64]={
    0,  0,  0,  0,  0,  0,  0,  0,
    5, 10, 10, 10, 10, 10, 10,  5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    0,  0,  0,  5,  5,  0,  0,  0
};

const float QueenSquare[64]={
    -20,-10,-10, -5, -5,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5,  5,  5,  5,  0,-10,
    -5,  0,  5,  5,  5,  5,  0, -5,
    0,  0,  5,  5,  5,  5,  0, -5,
    -10,  5,  5,  5,  5,  5,  0,-10,
    -10,  0,  5,  0,  0,  0,  0,-10,
    -20,-10,-10, -5, -5,-10,-10,-20
};

const float KingSquareMiddleGame[64]={
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -20,-30,-30,-40,-40,-30,-30,-20,
    -10,-20,-20,-20,-20,-20,-20,-10,
    20, 20,  0,  0,  0,  0, 20, 20,
    20, 30, 10,  0,  0, 10, 30, 20
};

const float KingSquareEndGame[64]={
    -50,-40,-30,-20,-20,-30,-40,-50,
    -30,-20,-10,  0,  0,-10,-20,-30,
    -30,-10, 20, 30, 30, 20,-10,-30,
    -30,-10, 30, 40, 40, 30,-10,-30,
    -30,-10, 30, 40, 40, 30,-10,-30,
    -30,-10, 20, 30, 30, 20,-10,-30,
    -30,-30,  0,  0,  0,  0,-30,-30,
    -50,-30,-30,-30,-30,-30,-30,-50
};

float ComputerAI_5::evaluatePosition(int ply , float alpha, float beta){
    int _bestMove;
    total_eval++;
    vector <int> choice= cp.getAllLegalMoves();
    if (choice.size()==0){
        if (cp.isCheck()){
            if (cp.colourToMove()==WHITE) return -10000;
            else return 10000;
        }
        else return 0;
    }
    sort(choice.begin(),choice.end(), choicePriority(cp));
    float v;
    if (cp.colourToMove()==WHITE){
        v=-11000;
        for (int choiceInd=0;choiceInd<choice.size();choiceInd++){

            
            MoveFootprint &fp=cp.makeMove(choice[choiceInd]/64, choice[choiceInd]%64, 'Q');

            float innerV;
            if (ply<=1 && fp.pieceOnEndPos==' ' && (!cp.isCheck() || ply<=-3)){
                innerV=evaluatePosition();
            }
            else innerV=evaluatePosition(ply-1, alpha, beta);
            if (innerV>9000) innerV--;
            if (innerV<-9000) innerV++;
            if (innerV>v) {
                _bestMove=choice[choiceInd];
            }
            v=std::max(v,innerV);
            alpha=std::max(alpha, v);
            cp.undoMove(fp);
            delete &fp;
            if (beta<=alpha) break;
            
        }
    }
    else {
        v=11000;
        for (int choiceInd=0;choiceInd<choice.size();choiceInd++){
            MoveFootprint &fp=cp.makeMove(choice[choiceInd]/64, choice[choiceInd]%64, 'q');
            float innerV;
            if (ply<=1 && (fp.pieceOnEndPos==' ') && (!cp.isCheck() || ply<=-3)){
                innerV=evaluatePosition();
            }
            else innerV=evaluatePosition(ply-1, alpha, beta);
            
            if (innerV>9000) innerV--;
            if (innerV<-9000) innerV++;
            
            if (innerV<v) {
                _bestMove=choice[choiceInd];
            }
            
            v=std::min(v,innerV);
            beta=std::min(beta, v);
            cp.undoMove(fp);
            delete &fp;
            if (beta<=alpha) break;
        }
    }
    
    bestMove=_bestMove;
    
    return v;
}

float ComputerAI_5::pieceValue(char c){
    switch (c){
        case 'P':
            return 1;
        case 'N':
            return 3.2;
        case 'B':
            return 3.3;
        case 'R':
            return 5;
        case 'Q':
            return 9;
        case 'K':
            return 10000;
    }
    return 0;
}

float ComputerAI_5::positionalBonus(char piece, int sqr, bool endGame){
    if (piece==toupper(piece)) sqr=63-sqr; // square values are reversed for black (and symmetric horizontally)
    switch (toupper(piece)) {
        case 'P':
            return PawnSquare[sqr]/100;
        case 'N':
            return KnightSquare[sqr]/100;
        case 'B':
            return BishopSquare[sqr]/100;
        case 'R':
            return RookSquare[sqr]/100;
        case 'Q':
            return QueenSquare[sqr]/100;
        case 'K':
            return (endGame?KingSquareEndGame[sqr]:KingSquareMiddleGame[sqr])/100;
    }
    return 0;
}

//heuristical evaluation, without looking ahead
float ComputerAI_5::evaluatePosition(){
    total_eval++;
    
    
    float eval;
    float whiteMaterialEval=0;
    float blackMaterialEval=0;
    for (int i=1;i<6;i++){
        whiteMaterialEval+=pieceValue(cp.pieceTypes[i])*cp.numPiecesOfType(cp.pieceTypes[i]);
    }
    for (int i=7;i<12;i++){
        blackMaterialEval+=pieceValue(toupper(cp.pieceTypes[i]))*cp.numPiecesOfType(cp.pieceTypes[i]);
    }
    
    
    
    //when should you start moving the king towards the centre?
    bool whiteEndGame=cp.numPiecesOfType('q')==0 && cp.numPiecesOfType('r')<=1;
    bool blackEndGame=cp.numPiecesOfType('Q')==0 && cp.numPiecesOfType('R')<=1;
    
    float whitePositionalEval=0;
    float blackPositionalEval=0;
    for (int i=0;i<12;i++){
        for (int j=0;j<cp.numPiecesOfType(cp.pieceTypes[i]);j++){
            if (i<6) whitePositionalEval+=positionalBonus(cp.pieceTypes[i], cp.getSquareOfPiece(cp.pieceTypes[i], j), whiteEndGame);
            else blackPositionalEval+=positionalBonus(cp.pieceTypes[i], cp.getSquareOfPiece(cp.pieceTypes[i], j), blackEndGame);
        }
    }
    //white has only a king
    if (whiteMaterialEval==0){
        if (cp.numPiecesOfType('q')>0 || cp.numPiecesOfType('r')>0)
        blackPositionalEval+=((float) 5)/((float) kingCageSize(WHITE));
    }
    if (blackMaterialEval==0){
        if (cp.numPiecesOfType('Q')>0 || cp.numPiecesOfType('R')>0)
        whitePositionalEval+=((float) 5)/((float) kingCageSize(BLACK));
    }
    
    eval=whiteMaterialEval+whitePositionalEval-blackMaterialEval-blackPositionalEval;
    //eval*=50/(whiteMaterialEval+blackMaterialEval); // incentivize trading down when ahead.
    return eval;
}

void ComputerAI_5::makeMove(){
       total_eval=0;
    cp.setupPosition(game.getCurrentPosition().getFEN());

    int depth=3;
    while (total_eval<MAX_EVAL){
        total_eval=0;
        float ev=evaluatePosition(depth);
        if (ev>9000 || ev<-9000) break;
        depth++;
    }
    
    game.makeMove(cp.ChessSquareConverti(bestMove/64), cp.ChessSquareConverti(bestMove%64), 'Q');
   
}

/*
 alpha beta pruning is most efficient when the best moves are evaluated relatatively quickly
 thus we order by the fast heuristic of biggest material captures
 */
bool choicePriority::operator() (int move1,int move2){
    int endValue1=ComputerAI::pieceValue(toupper(cp.getSquare(move1%64)));
    int endValue2=ComputerAI::pieceValue(toupper(cp.getSquare(move2%64)));
    if (endValue1>endValue2) return true;
    if (endValue2>endValue1) return false;
    int begValue1=ComputerAI::pieceValue(toupper(cp.getSquare(move1/64)));
    int begValue2=ComputerAI::pieceValue(toupper(cp.getSquare(move2/64)));
    return begValue1<begValue2;
}
//so far, the computer was dreadful at checkmating the lone king.
//so, we will incentivize it to make small king cages when only the king is left.
//checks how many squares the king of colour can go to if given infinity moves
int ComputerAI_5::kingCageSize(Colour colour){
    
    bool kingCanReach[64];
    for (int i=0;i<64;i++)  kingCanReach[i]=false;
    int numReach=1;
    std::queue<int> toExplore;
    toExplore.push(cp.getSquareOfPiece(colour==WHITE?'K':'k', 0));
    kingCanReach[toExplore.front()]=true;
    //int lastKSqr=toExplore.front();
    //int lastKSqrPiece=' ';
    int initialKSqr=toExplore.front();
    int initialCanEnPassantAt=cp.canEnPassantAt;
    cp.canEnPassantAt=-1;
    Colour initialColour=cp.colourToMove();
    cp.setColourToMove(colour);
    cp.setSquare(initialKSqr, ' ');
    while (!toExplore.empty()){
        int ksqr=toExplore.front();
        char lastPiece=cp.getSquare(ksqr);
        cp.setSquare(ksqr,colour==WHITE?'K':'k');
        vector<int> legalMoves=cp.getLegalMovesFrom(ksqr);
        for (int i=0;i<legalMoves.size();i++){
            if (!kingCanReach[legalMoves[i]]){
                kingCanReach[legalMoves[i]]=true;
                toExplore.push(legalMoves[i]);
                numReach++;
            }
        }
        cp.setSquare(ksqr, lastPiece);
        toExplore.pop();
    }
    cp.setSquare(initialKSqr, colour==WHITE?'K':'k');
    cp.setColourToMove(initialColour);
    cp.canEnPassantAt=initialCanEnPassantAt;
    total_eval+=numReach*5;
    return numReach;
}

