//
//  computer4.cpp
//  PawnPusher9000
//
//  Created by Lavi on 2015-11-28.
//  Copyright © 2015 Lavi. All rights reserved.
//

#include "computer4.h"
#include <ctype.h>
#include <cstdlib>
#include <time.h>
#include <iostream>

int ComputerAI_4::evaluatePosition(ChessPosition & cp, float ply){
    if (ply<=0){
        return evaluatePosition(cp);
    }
    vector <int> choice= cp.getAllLegalMoves();
    if (choice.size()==0){
        if (cp.isCheck()){
            if (cp.colourToMove()==WHITE) return -10000;
            else return 10000;
        }
        else return 0;
    }
    int best=(cp.colourToMove()==WHITE)?-11000:11000;
    for (int choiceInd=0;choiceInd<choice.size();choiceInd++){
        string begPos=cp.ChessSquareConverti(choice[choiceInd]/64);
        string endPos=cp.ChessSquareConverti(choice[choiceInd]%64);
        MoveFootprint &fp=cp.makeMove(begPos, endPos);
        int ev;
        //extra calculation for interesting variations
        if (fp.pieceOnEndPos!=' ' || cp.isCheck()) ev=evaluatePosition(cp, ply-0.9);
        else ev=evaluatePosition(cp,ply-1);
        cp.undoMove(fp);
        delete &fp;
        
        
        //faster mates are better/worse
        if (ev>9000){
            ev--;
        }
        if (ev<-9000){
            ev++;
        }
        
        if ((cp.colourToMove()==WHITE && best< ev) || (cp.colourToMove()==BLACK && best> ev)) best=ev;
    }
    return best;
}

//heuristical evaluation, without looking ahead
int ComputerAI_4::evaluatePosition(ChessPosition & cp){
    int eval=0;
    //pure material evaluation
    for (int i=0;i<cp.pieceTypes.length();i++){
        eval+=pieceValue(toupper(cp.pieceTypes[i]))*cp.numPiecesOfType(cp.pieceTypes[i])*(i<6?1:-1);
    }
    return eval;
}

void ComputerAI_4::makeMove(){
    srand(time(NULL));
    ChessPosition cp=game.getCurrentPosition();
    vector<int> choice =cp.getAllLegalMoves();
    /*for (int i=0;i<choice.size();i++){
        //cout << choice[i]/64 << " " << choice[i]%64 << endl;
        cout << cp.ChessSquareConverti(choice[i]/64) << " " << cp.ChessSquareConverti(choice[i]%64) << endl;
    }*/
    vector<float> score(choice.size());
    int maxScore=(cp.colourToMove()==WHITE)?-11000:11000;
    int maxScoreCounter=0;
    for (int choiceInd=0;choiceInd<choice.size();choiceInd++){
        string begPos=cp.ChessSquareConverti(choice[choiceInd]/64);
        string endPos=cp.ChessSquareConverti(choice[choiceInd]%64);
        MoveFootprint & fp=cp.makeMove(begPos,endPos);
        score[choiceInd]=evaluatePosition(cp, 2);
        cp.undoMove(fp);
        delete &fp;
        if ((cp.colourToMove()==WHITE && score[choiceInd]>maxScore) || (cp.colourToMove()==BLACK && score[choiceInd]<maxScore)) {
            maxScore=score[choiceInd];
            maxScoreCounter=1;
        }
        else if(score[choiceInd]==maxScore){
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





