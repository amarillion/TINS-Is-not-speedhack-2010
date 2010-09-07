#include "position.h"

POSITION::POSITION(int _x, int _y) : x(_x), y(_y)
{
}

POSITION POSITION::move(DIRECTION d)
{
    switch(d)
    {
      case NONE:
        break;
      case N:
        y--;
        break;
      case S:
        y++;
        break;
      case W:
        x--;
        break;
      case E:
        x++;
        break;
    }
    return *this;
}
