#include "movement.h"

void update_xy(DIRECTION d, POSITION max, POSITION &in)
{
    switch (d)
    {
      case NONE:
        break;
      case N:
        in.y--;
        if (in.y < 0) in.y += max.y;
        break;
      case E:
        in.x++;
        if (in.x >= max.x) in.x -= max.x;
        break;
      case S:
        in.y++;
        if (in.y >= max.y) in.y -= max.y;
        break;
      case W:
        in.x--;
        if (in.x < 0) in.x += max.x;
        break;
    };
}
