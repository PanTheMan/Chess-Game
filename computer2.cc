//
//  computer2.cpp
//  PawnPusher9000
//
//  Created by Lavi on 2015-11-28.
//  Copyright © 2015 Lavi. All rights reserved.
//

#include "computer2.h"
#include <ctype.h>
#include <cstdlib>
#include <time.h>

void ComputerAI_2::makeMove(){
    srand(time(NULL));
    ChessPosition cp=game.getCurrentPosition();
    vector<int> choice =cp.getAllLegalMoves();
    vector<float> score(choice.size());
    int maxScore=-100;
    int maxScoreCounter=0;
    for (int choiceInd=0;choiceInd<choice.size();choiceInd++){
        string begPos=cp.ChessSquareConverti(choice[choiceInd]/64);
        string endPos=cp.ChessSquareConverti(choice[choiceInd]%64);
        score[choiceInd]=pieceValue(cp.getSquare(endPos));
        MoveFootprint & fp=cp.makeMove(begPos,endPos);
        
        
        
        
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
