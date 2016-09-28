//
//  computer1.hpp
//  PawnPusher9000
//
//  Created by Lavi on 2015-11-27.
//  Copyright © 2015 Lavi. All rights reserved.
//

#ifndef computer1_hpp
#define computer1_hpp
#include "computer.h"
class ComputerAI_1: public ComputerAI{
public:
    ComputerAI_1(ChessGame & cg):ComputerAI(cg){}
    ~ComputerAI_1(){}
    void makeMove();
};

#endif /* computer1_hpp */
