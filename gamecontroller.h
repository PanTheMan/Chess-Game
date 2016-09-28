//
//  GameController.hpp
//  PawnPusher9000
//
//  Created by Lavi on 2015-11-22.
//  Copyright © 2015 Lavi. All rights reserved.
//

#ifndef GameController_hpp
#define GameController_hpp
#include <iostream>
#include <string>
#include "game.h"
#include "textdisplay.h"

#include "player.h"
#include "human.h"

using namespace std;
const int maxPlayer = 2;
const int boardSize = 8;

class GameController : public NotifiableController{
    View *view;
	View *gd;
    ChessGame *game;
    Player *players[maxPlayer]; //players[0]=white, players[1]=black
    bool autoMove;
    bool gameInProg;
    void makeMove();
public:
	GameController();
	~GameController();
	void fileSetup(char *fileName); 
    void play(char *fileName=NULL);
	//void play(std::string fileName="");
	void resign();
	void init(istream & input);
    void notifySquareChange(string sqrName, char newValue);
    void notifyMoveChange(string begPos, string endPos, string checkPos, bool isEndGame);
};


#endif /* GameController_hpp */
