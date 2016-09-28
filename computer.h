//
//  ComputerAI.hpp
//  PawnPusher9000
//
//  Created by Lavi on 2015-11-27.
//  Copyright © 2015 Lavi. All rights reserved.
//

#ifndef ComputerAI_hpp
#define ComputerAI_hpp
#include "player.h"

class ComputerAI : public Player{
    
    
    
public:
    static float pieceValue(char c);
    ComputerAI(ChessGame &cg):Player(cg){
		isHuman = false;        
    }
    virtual ~ComputerAI(){}
    virtual void makeMove()=0;
};

#endif /* ComputerAI_hpp */
