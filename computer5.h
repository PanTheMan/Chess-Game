//
//  computer5.hpp
//  PawnPusher9000
//
//  Created by Lavi on 2015-12-02.
//  Copyright © 2015 Lavi. All rights reserved.
//

#ifndef computer5_hpp
#define computer5_hpp
#include "computer.h"
#include <stack>



class ComputerAI_5: public ComputerAI{
    const int MAX_EVAL;
    ChessPosition cp;
    int bestMove;
public:
    ComputerAI_5(ChessGame & cg):ComputerAI(cg),MAX_EVAL(20000),cp(cg.getCurrentPosition()){
    }
    ~ComputerAI_5(){}
    void makeMove();
    float evaluatePosition(int ply, float alpha=-11000, float beta=11000);
    float evaluatePosition();
    static float pieceValue(char c);
    float positionalBonus(char piece, int sqr, bool endGame);
    int kingCageSize(Colour);
};

struct choicePriority{
    const ChessPosition &cp;
    choicePriority(ChessPosition & _cp):cp(_cp){}
    bool operator() (int move1,int move2);
};

#endif /* computer5_hpp */
