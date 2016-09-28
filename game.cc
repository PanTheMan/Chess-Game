//
//  game.cpp
//  PawnPusher9000
//
//  Created by Lavi on 2015-11-22.
//  Copyright © 2015 Lavi. All rights reserved.
//

#include "game.h"
#include <iostream>

ChessPosition & ChessGame::getCurrentPosition(){
	return *cp;
}

int ChessGame::totalGames(){
	return blackWins+whiteWins;
}

ChessGame::ChessGame(){
	blackWins=0;
	whiteWins=0;
	moveNum=0;
	cp = new ChessPosition();
	controller = NULL;	
}

ChessGame::~ChessGame(){
	delete cp;
	while(!sfp.empty()){
		MoveFootprint * fp = sfp.top();
		sfp.pop();
		delete fp;
	}
}

/*
 * Returns whether successful
 * If move is legal, we get a new position with the move made, and add it to the front of the linked list
 * It also updates controller with the changed squares
 */
bool ChessGame::makeMove(string begPos, string endPos, char promotion){
	
    if (getCurrentPosition().isLegalMove(begPos, endPos)){
 		MoveFootprint &fp =cp->makeMove(begPos, endPos, promotion);
        sfp.push(&fp);
		controller->notifySquareChange(begPos, cp->getSquare(begPos));
        controller->notifySquareChange(endPos, cp->getSquare(endPos));
		// Check for castling
		if(cp->getSquare(endPos)=='K'|| cp->getSquare(endPos)=='k'){
			int diff = begPos.at(0)-endPos.at(0);
			string prevRookPos="",newRookPos;
			char row =endPos.at(1) ;
		//king castled queen side
			if(diff == 2){
				prevRookPos = "a1";
				prevRookPos.replace(1,1,1,row);
				newRookPos = "d1";
				newRookPos.replace(1,1,1,row);
			}
			else if(diff == -2){
				prevRookPos = "h1";
				prevRookPos.replace(1,1,1,row);
				newRookPos = "f1";			
				newRookPos.replace(1,1,1,row);
			}
            if (prevRookPos!=""){
                controller->notifySquareChange(prevRookPos, cp->getSquare(prevRookPos));
                controller->notifySquareChange(newRookPos, cp->getSquare(newRookPos));
            }
		}
        if (cp->getSquare(endPos)=='p' || cp->getSquare(endPos)=='P'){
            if (begPos[0]!=endPos[0] && fp.pieceOnEndPos==' '){
                //must be en passant
                string toUpdate=endPos;
                if (toUpdate[1]=='6') toUpdate[1]--;
                else toUpdate[1]++;
                controller->notifySquareChange(toUpdate, cp->getSquare(toUpdate));
            }
        }

        controller->notifyMoveChange(begPos, endPos, cp->isCheck()?cp->ChessSquareConverti(cp->getSquareOfPiece(cp->colourToMove()==WHITE?'K':'k', 0)):"", isWon(1-cp->colourToMove()));
		
        return true;
    }
    else {
			cout << "Invalid Chess move. Please check and try again" << endl;        
            return false;
    }
}

bool ChessGame::isWon(int player,bool resign){

	int win;
	if(resign == true){
		if(player == WHITE) player = BLACK;
		else player = WHITE;
		win = CHECKMATE;
	}
	else{ 
		win = cp->flag();

	}
	if(win == CHECKMATE){
		if(!resign)
			cout << "Checkmate! ";
		if(player==WHITE) {
			whiteWins++;
			cout << "White wins!" << endl;;
		}
		else {
			blackWins++;
			cout << "Black wins!" << endl;;
		}
		return true;
	}
	if(win == STALEMATE){
		whiteWins += 0.5;
		blackWins += 0.5;
		cout << "Stalemate!" << endl;
		return true;
	}
	if(win	== CHECK){
		if(player == WHITE)	
			cout << "Black is in check" << endl;
		else
			cout << "White is in check" << endl;
	}

	return false;
}

bool ChessGame::undoMove(){
    if (sfp.empty()) return false;
    MoveFootprint &footprint=*sfp.top();

    cp->undoMove(footprint);
    Colour colour = cp->colourToMove();
    string begPos=ChessPosition::ChessSquareConverti(footprint.begpos);
    string endPos=ChessPosition::ChessSquareConverti(footprint.endpos);
    controller->notifySquareChange(begPos, cp->getSquare(begPos));
    controller->notifySquareChange(endPos, cp->getSquare(endPos));
    
    
    
    if (cp->getSquare(begPos)=='K'){
        if (begPos=="e1" && endPos=="g1"){
            controller->notifySquareChange("h1", 'R');
            controller->notifySquareChange("f1", ' ');
        }
        if (begPos=="e1" && endPos=="c1"){
            controller->notifySquareChange("a1", 'R');
            controller->notifySquareChange("d1", ' ');
        }
    }
    if (cp->getSquare(begPos)=='k'){
        if (begPos=="e8" && endPos=="g8"){
            controller->notifySquareChange("h8", 'r');
            controller->notifySquareChange("f8", ' ');
        }
        if (begPos=="e8" && endPos=="c8"){
            controller->notifySquareChange("a8", 'r');
            controller->notifySquareChange("d8", ' ');
        }
    }
    if (footprint.canEnPassantAt!=-1){
        string sqrn="  ";
        sqrn[0]=footprint.canEnPassantAt+'a';
        sqrn[1]=(colour==WHITE?'5':'4');
        controller->notifySquareChange(sqrn, cp->getSquare(sqrn));
    }
    sfp.pop();
    delete &footprint;
    
    controller->notifyMoveChange("", "", cp->isCheck()?cp->ChessSquareConverti(cp->getSquareOfPiece(cp->colourToMove()==WHITE?'K':'k', 0)):"", isWon(1-cp->colourToMove()));
    
    return true;
    

}


void ChessGame::setPosition(){
	delete cp;
	cp = new ChessPosition();
    for (string sqrn="a1";sqrn[1]<='8';sqrn[1]++){
        for (sqrn[0]='a';sqrn[0]<='h';sqrn[0]++){
            controller->notifySquareChange(sqrn, cp->getSquare(sqrn));
        }
    }
	while(!sfp.empty()){
        MoveFootprint * fp = sfp.top();
        sfp.pop();
        delete fp;
    }
	
}

void ChessGame::finalScore(){
	cout << "Final Score:" << endl;
	cout << "White: " << whiteWins << endl;
	cout << "Black: " << blackWins << endl;
}

void ChessGame::setControl(NotifiableController *nc){
	controller = nc;	
}
