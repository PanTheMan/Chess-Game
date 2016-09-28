//
//  computer1.cpp
//  PawnPusher9000
//
//  Created by Lavi on 2015-11-27.
//  Copyright © 2015 Lavi. All rights reserved.
//

#include "computer1.h"
#include <cstdlib>
#include <time.h>



void ComputerAI_1::makeMove(){
    ChessPosition &cp=game.getCurrentPosition();
    srand(time(NULL));
    string begPos="  ", endPos="  ";
    vector<int> allLegalMoves=cp.getAllLegalMoves();
    
    int chosen=allLegalMoves[rand()%allLegalMoves.size()];
    int b=chosen/64;
    int e=chosen%64;
    begPos[0]=b%8+'a';
    begPos[1]=b/8+'1';
    endPos[0]=e%8+'a';
    endPos[1]=e/8+'1';
    game.makeMove(begPos, endPos);
}
