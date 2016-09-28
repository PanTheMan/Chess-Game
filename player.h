//
//  player.hpp
//  PawnPusher9000
//
//  Created by Lavi on 2015-11-22.
//  Copyright © 2015 Lavi. All rights reserved.
//

#ifndef player_hpp
#define player_hpp
#include "game.h"
class Player{
protected:
     ChessGame & game; //reference to the game
public:
	bool isHuman;


    Player(ChessGame & g):game(g){
    }
    virtual ~Player(){}
    virtual void makeMove()=0;
};


#endif /* player_hpp */
