//
//  computer3.hpp
//  PawnPusher9000
//
//  Created by Lavi on 2015-11-27.
//  Copyright © 2015 Lavi. All rights reserved.
//

#ifndef computer3_hpp
#define computer3_hpp
#include "computer.h"
class ComputerAI_3: public ComputerAI{
public:
    ComputerAI_3(ChessGame & cg):ComputerAI(cg){
        
    }
    ~ComputerAI_3(){}
    void makeMove();
};

#endif /* computer3_hpp */
