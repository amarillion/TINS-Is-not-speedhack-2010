#include "direction.h"

DIRECTION invert(DIRECTION d)
{
    switch (d)
    {
      case NONE:    return NONE;
      case N:       return S;
      case E:       return W;
      case S:       return N;
      case W:       return E;
    };
    return NONE;
}
