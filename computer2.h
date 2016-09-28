//
//  computer2.hpp
//  PawnPusher9000
//
//  Created by Lavi on 2015-11-28.
//  Copyright © 2015 Lavi. All rights reserved.
//

#ifndef computer2_hpp
#define computer2_hpp
#include "computer.h"
class ComputerAI_2: public ComputerAI{
public:
    ComputerAI_2(ChessGame & cg):ComputerAI(cg){
        
    }
    void makeMove();
};


#endif /* computer2_hpp */
