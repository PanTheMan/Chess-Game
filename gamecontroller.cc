
///
//  GameController.cpp
//  PawnPusher9000
//
//  Created by Lavi on 2015-11-22.
//  Copyright © 2015 Lavi. All rights reserved.
//
#include <cstring>
#include <fstream>
#include "gamecontroller.h"
#include "computer.h"
#include "computer1.h"
#include "computer2.h"
#include "computer3.h"
#include "computer4.h"
#include "computer5.h"
#include "graphicdisplay.h"
using namespace std;

GameController::GameController(){
	game = new ChessGame();
	view = NULL;
	gd = NULL;
	players[0]= NULL;
	players[1] = NULL;
    autoMove=false;
    gameInProg=false;

}

void GameController::notifySquareChange(string sqrName, char newValue){
	// Call the text and graphics notify
	view->notifySquareChange(sqrName, newValue);
	gd->notifySquareChange(sqrName,newValue);
}
void GameController::notifyMoveChange(string begPos, string endPos, string checkPos, bool isEndGame){
	if(begPos=="resign"){
		resign();
		return;
	}
    view->notifyMoveChange( begPos,endPos,checkPos);
    gd->notifyMoveChange( begPos,endPos,checkPos);
    view->print(cout);
    if (isEndGame){
        delete view;
        //delete gd;
        view = NULL;
        //gd = NULL;
        gameInProg = false;
    }
}

void GameController::init(istream & input){
	string colour ,command, position;
	char piece;
	while(true){
		view->print(cout);
		input >> command;
		ChessPosition & cp = game->getCurrentPosition();
		if(command == "+"){
			// NEED TO ASK SANITY CHECKS
			input >> piece;
			input >> position;
			notifySquareChange(position,piece);
			// Call game to change
			cp.setSquare(position,piece);
		}else if(command == "="){
			input >> colour;
			// Call chessposition to set colour
			if(colour == "black"){
				cp.setColourToMove(true);
			}else 
				cp.setColourToMove(false);
		}else if(command == "-"){
			input >> position;
		// Notify view and graphics
			notifySquareChange(position,empty);

			
			cp.setSquare(position,empty);
		}else if(command == "done"){
			if(cp.isLegalPosition() && !cp.isCheck())
				break;
			else{
				cout << "Can't leave setup yet. Board is invalid" << endl;
			}
		}else if (command == "FEN"){
			string fenStr;
            string fenStrPart;
            for (int i=0;i<6;i++){
                input >> fenStrPart;
                fenStr+=fenStrPart+" ";
            }
			cp.setupPosition(fenStr);
            for (string sqrn="a1"; sqrn[0]<='h';sqrn[0]++){
                for (sqrn[1]='1';sqrn[1]<='8';sqrn[1]++){
                    notifySquareChange(sqrn, cp.getSquare(sqrn));
                }
            }
		}
		
	}
}

// Function to setup from a file
void GameController::fileSetup(char *fileName){
        char sqr;
        ifstream file;
        file.open(fileName);

        for(int i=0;i<boardSize;i++){
                for(int j=0;j<boardSize;j++){
	
                        sqr = file.get();
						if(sqr == '\n') sqr=file.get();						
                        string pos = "a8";
                        pos.replace(0,1,1,pos.at(0)+j);
                        pos.replace(1,1,1,pos.at(1)-i);
						if(sqr==blk) sqr = empty;						
                        notifySquareChange(pos,sqr);
						
                     game->getCurrentPosition().setSquare(pos,sqr);
                }
        }
        sqr = file.get();
		sqr = file.get();
		
        if(sqr == 'W'){
				cout << "White to move first " << endl;
                game->getCurrentPosition().setColourToMove(WHITE);
        }else{
				cout << "Black to move first " << endl;
                game->getCurrentPosition().setColourToMove(BLACK);
		}
	file.close();
	view->print(cout);
}

void GameController::resign(){
            if(game->getCurrentPosition().colourToMove()==WHITE){

                game->isWon(WHITE,true);

            }
            else{
                game->isWon(BLACK,true);
            }
            // Delete the views
            delete view;
            view = NULL;
            delete gd;
            gd = NULL;

}

// Main control to play game
void GameController::play(char *fileName){
	string cmd;
	gameInProg = false;
	game->setControl(this);
	if(fileName) {
        delete gd;
		gd = new GraphicDisplay();
		view = new TextDisplay();
		fileSetup(fileName);
	}
	while(cin >> cmd && !cin.eof()){
		if(!gameInProg){
		if(cmd == "game"){
			gameInProg = true;

			if(!view){
				view = new TextDisplay();
                delete gd;
				gd = new GraphicDisplay();
				game->setPosition();
			}
			string player;
			delete players[0];
            delete players[1];
			// Setup player objects
			for(int i=0; i<maxPlayer;i++){
				cin >> player;
				if(player == "human"){
					players[i] = new Human(*game);
				}
                else if(player=="computer1"){

                    players[i]= new ComputerAI_1(*game);
                }
                else if (player=="computer2"){

                    players[i]= new ComputerAI_2(*game);
                }
                else if(player=="computer3"){

                    players[i]= new ComputerAI_3(*game);
                }
				else if(player == "computer4"){

					players[i] = new ComputerAI_4(*game);
				}
                else if(player == "computer5"){

                    players[i] = new ComputerAI_5(*game);
                }

			}
		}
		else if(cmd == "setup"){

		if(!fileName || (fileName!="" && game->totalGames()!=0)){
			if(!view){
				view = new TextDisplay();
                delete gd;
				gd = new GraphicDisplay();
			}
		//reset  position
			game->setPosition();
		}else fileName = "";
			init(cin);
		}else{
			cout << "Game should be started first/Invalid command" << endl;
		}
		}else{
       if(cmd == "resign"){
        	gameInProg = false;
			resign();
/*getCurrentPosition().colourToMove()==WHITE){
                
				game->isWon(WHITE,true);
				
			}
            else{
                game->isWon(BLACK,true);
			}
			// Delete the views
			delete view;
			view = NULL;
			delete gd;
			gd = NULL;*/
        }
        else if(cmd == "move"){
            makeMove();
        }
		else if(cmd == "undo"){
			if(game->undoMove()==false) cout << "Can't undo anything" << endl;
			else {
				view->print(cout);
			}
		}
        else if(cmd == "auto"){
            autoMove=!autoMove;
        }
        else{
			cout << "Invalid command" << endl;
		}	
		}
	}
	game->finalScore();
}

void GameController::makeMove(){
    do {
        int player;
        if (game->getCurrentPosition().colourToMove()==WHITE){
			players[WHITE]->makeMove();
            player=WHITE;
            if(players[BLACK]->isHuman) break;
        }
        else {
            
            player=BLACK;
            players[BLACK]->makeMove();
			if(players[WHITE]->isHuman) break;
        }
    } while (autoMove && gameInProg);
    
}

// Destructor
GameController::~GameController(){
	    delete view;
		delete gd;
	
        delete game;
	
		delete players[0];
		delete players[1];
}

