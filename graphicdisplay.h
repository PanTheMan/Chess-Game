#ifndef GRAPHICDISPLAY_H
#define GRAPHICDISPLAY_H
#include <iostream>
#include "window.h"
#include <sstream>
#include <string>
#include "view.h"


// Constants meant to be added to each square to place text in the center
const int x_center = 14;
const int y_center = 36;
//const std::string stdBoard = "RNbqkbnrpppppppp _ _ _ __ _ _ _  _ _ _ __ _ _ _ PPPPPPPPRNBQKBNR";
const std::string backRow[8] = {"R","N","B","Q","K","B","N","R"};
const std::string gridLet[8] = {"A","B","C","D","E","F","G","H"};
const std::string gridNum[8] = {"8","7","6","5","4","3","2","1"};
class GraphicDisplay : public View {
    char **theDisplay;
	Xwindow *w;
 // char **theDisplay;          //the 8 x 8 chessboard
    std::string highlights[3];
    
    void redrawSquare(int row, int column, int colour);
    void highlight(std::string sqr, bool check);
    void unhighlight(std::string sqr);
     
 public:
  GraphicDisplay(); //one arg constructor where the parameter is the gridSize

  ~GraphicDisplay(); //dtor
  bool checkBlkSqrColour(std::string sqrName);

  /*
   * The Controller calls notify to update the (row,column) location to be ch
   */

  void notifySquareChange(std::string sqrName, char ch);
    void notifyMoveChange(std::string begPos, std::string endPos, std::string checkPos);
  /*
   * Prints the grid as specified in the assignment specification.
   */
  void print(std::ostream &out) const ;
};

#endif

