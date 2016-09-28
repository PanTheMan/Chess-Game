//
//  human.hpp
//  PawnPusher9000
//
//  Created by Lavi on 2015-11-22.
//  Copyright © 2015 Lavi. All rights reserved.
//

#ifndef human_hpp
#define human_hpp
#include "player.h"
#include <iostream>
class Human : public Player {

public:
    Human(ChessGame &);
    ~Human(){
        
    }
    void makeMove();
};

#endif /* human_hpp */
