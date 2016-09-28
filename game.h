//
//  game.hpp
//  PawnPusher9000
//
//  Created by Lavi on 2015-11-22.
//  Copyright © 2015 Lavi. All rights reserved.
//

#ifndef game_hpp
#define game_hpp
#include "position.h"
#include <stack>

struct NotifiableController {
    virtual void notifySquareChange(string sqrName, char newValue) = 0;
    virtual void notifyMoveChange(string begPos, string endPos, string checkPos, bool isEndGame) =0;
};

class ChessGame {
    int moveNum;
	float blackWins;
	float whiteWins;
    ChessPosition * cp;
    NotifiableController * controller;
	std::stack<MoveFootprint *>  sfp;
	
public:
    ChessGame();
    ~ChessGame();
	int totalGames();

    void setControl(NotifiableController *nc);
	void finalScore();
	void setPosition();
	bool undoMove();
	bool isWon(int colour,bool resign = false);
	void printScores();
    ChessPosition & getCurrentPosition(); //called by computer players to assess the position and do calculations
    bool makeMove(string begPos, string endPos, char promotion = ' ');
};

#endif /* game_hpp */
