#ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H
#include <iostream>
#include <sstream>
#include <string>
#include "view.h"

const std::string stdBoard = "rnbqkbnrpppppppp _ _ _ __ _ _ _  _ _ _ __ _ _ _ PPPPPPPPRNBQKBNR";
const char blk = '_';


class TextDisplay : public View {
  char **theDisplay;          //the 8 x 8 chessboard

 public:
  TextDisplay(); //Constructor
  ~TextDisplay(); //dtor
  bool checkBlkSqrColour(std::string sqrName);

  /*
   * The Controller calls notify to update the (row,column) location to be ch
   */
  void notifySquareChange(std::string sqrName, char ch);

  /*
   * Prints the grid as specified in the assignment specification.
   */
  void print(std::ostream &out) const ;
};

#endif
