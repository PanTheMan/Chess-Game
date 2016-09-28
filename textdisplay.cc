#include "textdisplay.h"
using namespace std;

// Constructor
TextDisplay::TextDisplay() :View(){
        // Allocate the 2D array of the board

        theDisplay = new char *[gridSize];
        for(int i=0; i< gridSize; i++){
        	theDisplay[i]=new char [gridSize];
			for(int j=0; j < gridSize; j++){
				theDisplay[i][j] = stdBoard.at(i*gridSize+j);
			}
        }
}

// Destructor
TextDisplay::~TextDisplay(){
        // Delete the 2D array
        for(int i=0;i<gridSize;i++){
                delete []theDisplay[i];
        }
        delete [] theDisplay;
}

bool TextDisplay::checkBlkSqrColour(string sqrName){
        int position = sqrName.at(0)-'a' + sqrName.at(1)-'1';
        if(position%2 == 0) return true;
                else return false;
}

// View was notified by controller object
// Update the theDisplay array with new state
void TextDisplay::notifySquareChange(string sqrName, char ch){
	int row = 8-(sqrName.at(1)-'0');
	int column =sqrName.at(0)-'a';
    if(ch==empty){
		if(checkBlkSqrColour(sqrName)) ch = blk;	
	}
	theDisplay[row][column] = ch;
}

// Called to print out the current board
void TextDisplay::print(ostream &out) const{
        // Cycle through the 2D array with 2 for loops
        for(int i=0;i<gridSize;i++){
				out << gridSize-i << " ";
                for(int j=0;j<gridSize;j++){
                        out << theDisplay[i][j];
                }
                // new line at the end
                out << endl;
        }
		out << endl;
		out << "  abcdefgh" << endl;
		
}

