//
//  human.cpp
//  PawnPusher9000
//
//  Created by Lavi on 2015-11-22.
//  Copyright © 2015 Lavi. All rights reserved.
//

#include "human.h"
#include <iostream>
#include <string>
using namespace std;

Human::Human(ChessGame & g) : Player(g){
    isHuman = true;
}

void Human::makeMove(){
    ChessPosition & board= game.getCurrentPosition();
    string begPos, endPos;
    if (cin >> begPos >> endPos){
		
        bool promotionNeeded=false;
        if (board.colourToMove()==0){
            if (board.getSquare(begPos)=='P' && endPos[1]=='8') promotionNeeded=true;
        }
        else {
            if (board.getSquare(endPos)=='p' && endPos[1]=='1') promotionNeeded=true;
        }
        if (promotionNeeded){
            char prom;
            cin >> prom;
            if (prom == 'Q' || prom == 'R' || prom == 'B' || prom=='N'){
                if (board.colourToMove()==BLACK){
                    prom+='a'-'A';
                }
                game.makeMove(begPos, endPos, prom);
            }
        }
        else game.makeMove(begPos, endPos);
    }
    
}
