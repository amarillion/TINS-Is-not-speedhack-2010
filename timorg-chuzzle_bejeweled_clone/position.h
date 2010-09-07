#ifndef POSITION_H
#define POSITION_H

#include "direction.h"

class POSITION
{
  public:
    POSITION(int _x = 0, int _y = 0);
    POSITION move(DIRECTION d);
    int x, y;

};

#endif
