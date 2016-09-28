//
//  ComputerAI.cpp
//  PawnPusher9000
//
//  Created by Lavi on 2015-11-27.
//  Copyright © 2015 Lavi. All rights reserved.
//

#include "computer.h"

float ComputerAI::pieceValue(char c){
    switch (c){
        case 'P':
            return 1;
        case 'N':
            return 3;
        case 'B':
            return 3;
        case 'R':
            return 5;
        case 'Q':
            return 9;
        case 'K':
            return 10000;
    }
    return 0;
}
