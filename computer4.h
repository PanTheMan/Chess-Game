//
//  computer4.hpp
//  PawnPusher9000
//
//  Created by Lavi on 2015-11-28.
//  Copyright © 2015 Lavi. All rights reserved.
//

#ifndef computer4_hpp
#define computer4_hpp
#include "computer.h"
class ComputerAI_4: public ComputerAI{
public:
    ComputerAI_4(ChessGame & cg):ComputerAI(cg){
        
    }
    ~ComputerAI_4(){}
    void makeMove();
    int evaluatePosition(ChessPosition & cp, float ply);
    int evaluatePosition(ChessPosition & cp);
};

#endif /* computer4_hpp */
