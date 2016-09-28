//
//  main.cpp
//  PawnPusher9000
//
//  Created by Lavi on 2015-11-22.
//  Copyright © 2015 Lavi. All rights reserved.
//

#include <iostream>
#include "gamecontroller.h"
#include "game.h"
//#include "computer5.h"
int main(int argc, char * argv[]) {
	GameController gc;
    ChessPosition cp;
    
    /*ChessGame cg;
    cg.getCurrentPosition().setupPosition("rnbqkb1r/pppppppp/5n2/8/3PP3/8/PPP2PPP/RNBQKBNR b KQkq - 0 2");
    ComputerAI_5 ai(cg);
    cout << ai.evaluatePosition();*/
    
    
	if(argc == 2)
		gc.play(argv[1]);
	else
		gc.play();
}
