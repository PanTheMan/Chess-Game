//
//  computer3.cpp
//  PawnPusher9000
//
//  Created by Lavi on 2015-11-27.
//  Copyright © 2015 Lavi. All rights reserved.
//

#include "computer3.h"
#include <ctype.h>
#include <cstdlib>
#include <time.h>


void ComputerAI_3::makeMove(){
    srand(time(NULL));
    ChessPosition cp=game.getCurrentPosition();
    vector<int> choice =cp.getAllLegalMoves();
    vector<float> score(choice.size());
    int maxScore=-100;
    int maxScoreCounter=0;
    for (int choiceInd=0;choiceInd<choice.size();choiceInd++){
        int begPos=choice[choiceInd]/64;
        int endPos=choice[choiceInd]%64;
        score[choiceInd]=pieceValue(toupper(cp.getSquare(endPos)));
        MoveFootprint & fp=cp.makeMove(begPos,endPos);
        
        int loss=0;
        for (int i=(cp.colourToMove()==WHITE?6:0);i<(cp.colourToMove()==WHITE?12:6);i++){
            for (int j=0;j<cp.numPiecesOfType(cp.pieceTypes[i]);j++){
                if (cp.getLegalMovesTo(cp.getSquareOfPiece(cp.pieceTypes[i], j)).size()>0){
                    loss=pieceValue(toupper(cp.pieceTypes[i]));
                    break;
                }
            }
            if (loss>0) break;
        }
        
        score[choiceInd]-=loss;
        
        
        switch (cp.flag()){
            case CHECK:
                score[choiceInd]+=0.2;
                break;
            case CHECKMATE:
                score[choiceInd]=10000;
                break;
            default:
                break;
        }
        
        cp.undoMove(fp);
        delete &fp;
        
        if (score[choiceInd]>maxScore) {
            maxScore=score[choiceInd];
            maxScoreCounter=1;
        }
        else if (score[choiceInd]==maxScore){
            maxScoreCounter++;
        }
    }
    int finalChoice=rand()%maxScoreCounter;
    for (int choiceInd=0;choiceInd<choice.size();choiceInd++){
        if (score[choiceInd]==maxScore){
            if (finalChoice==0){
                string begPos=cp.ChessSquareConverti(choice[choiceInd]/64);
                string endPos=cp.ChessSquareConverti(choice[choiceInd]%64);
                game.makeMove(begPos, endPos);
                break;
            }
            else finalChoice--;
        }
    }
}






