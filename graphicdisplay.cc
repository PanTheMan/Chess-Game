#include "graphicdisplay.h"

using namespace std;

void GraphicDisplay::print(ostream &out) const{
	return ;
}

// Constructor
GraphicDisplay::GraphicDisplay() :View(){
    theDisplay = new char *[gridSize];
    for(int i=0; i< gridSize; i++){
        theDisplay[i]=new char [gridSize];
    }
    for (int i=0;i<3;i++){
        highlights[i]="";
    }
    
        // Allocate the 2D array of the board
	w = new Xwindow(500,500);
	/*for(int i=0;i<gridSize;i++){
		for(int j=0;j<gridSize;j++){
			if((i*gridSize + j+i)%2 ==0)
				w->fillRectangle(j*50,i*50,50,50,Xwindow::White);
			else
				w->fillRectangle(j*50,i*50,50,50,Xwindow::Black);
		}
	}*/


	// Set up grid    
	for(int a=0;a<gridSize;a++){
		w->drawBigString(400+x_center,50*a+y_center,gridNum[a],Xwindow::Black);
		/*w->drawBigString(50*a+x_center,y_center,backRow[a],Xwindow::Brown);
    	w->drawBigString(50*a+x_center, 50+y_center, "P", Xwindow::Brown);

		w->drawBigString(50*a+x_center, 300+y_center, "P", Xwindow::Royal);
		w->drawBigString(50*a+x_center,350+y_center,backRow[a],Xwindow::Royal);*/
		w->drawBigString(50*a+x_center,400+y_center,gridLet[a],Xwindow::Black);
	}
	w->drawString(50,450,"This colour represents black", Xwindow::Brown);
	w->drawString(50,470, "This colour represents white", Xwindow::Royal);
	
}


// Destructor
GraphicDisplay::~GraphicDisplay(){
        // Delete the Xwindow
    delete w;
    for(int i=0;i<gridSize;i++){
        delete []theDisplay[i];
    }
    delete [] theDisplay;
}

void GraphicDisplay::redrawSquare(int row, int column, int colour){
    char ch=theDisplay[row][column];
    w->fillRectangle(column*50,row*50,50,50,colour);
    int colourPiece = Xwindow::Royal;
    if(ch!=empty){
        if(ch<123 && ch >97){
            ch -=32;
            colourPiece=Xwindow::Brown;
        }
        string piece=" ";
        piece.replace(0,1,1,ch);
        w->drawBigString(50*column+x_center, 50*row+y_center, piece,colourPiece);
    }
}


// Checks what colour hte square is at sqrName
bool GraphicDisplay::checkBlkSqrColour(string sqrName){
        int position = sqrName.at(0)-'a' + sqrName.at(1)-'1';
        if(position%2 == 0) return true;
                else return false;
}

// Changes the square at sqrName to ch
void GraphicDisplay::notifySquareChange(string sqrName, char ch){
	 int row = 8-(sqrName.at(1)-'0');
     int column =sqrName.at(0)-'a';
    theDisplay[row][column]=ch;
    unhighlight(sqrName);
}

void GraphicDisplay::highlight(string sqr,bool check){
    if (sqr=="") return;
    int row = 8-(sqr.at(1)-'0');
    int column =sqr.at(0)-'a';
    int colour;
    if (check){
        colour= Xwindow::Magenta;
    }
    else if(checkBlkSqrColour(sqr)){
        colour = Xwindow::Green;
    }
    else{
        colour = Xwindow::Yellow;
    }
    redrawSquare(row, column, colour);
}

void GraphicDisplay::unhighlight(string sqr){
    if (sqr=="") return;
    int row = 8-(sqr.at(1)-'0');
    int column =sqr.at(0)-'a';
    int colour;
    if(checkBlkSqrColour(sqr)){
        colour = Xwindow::Black;
    }
    else{
        colour = Xwindow::White;
    }
    redrawSquare(row, column, colour);
}

void GraphicDisplay::notifyMoveChange(string begPos, string endPos, string checkPos){
    for (int i=0;i<3;i++){
        unhighlight(highlights[i]);
    }
    highlight(begPos, false);
    highlight(endPos, false);
    highlight(checkPos, true);
    highlights[0]=begPos;
    highlights[1]=endPos;
    highlights[2]=checkPos;
}
	
