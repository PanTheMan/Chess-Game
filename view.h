#ifndef DISPLAY_H
#define DISPLAY_H
#include <iostream>
#include <sstream>
#include <string>

const char empty = ' ';

class View {

  protected:
    const int gridSize;
  public:
    View();

 /*
  * The Controller calls notify to update the square location to be ch
  */
 virtual void notifySquareChange(std::string sqrName, char ch) = 0;

    virtual void notifyMoveChange(std::string begPos, std::string endPos, std::string checkPos) {};

 virtual void print(std::ostream &out) const = 0;

 virtual ~View() = 0;

};

#endif
