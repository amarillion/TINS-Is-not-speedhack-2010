#ifndef PHYSICS_TILE_H
#define PHYSICS_TILE_H

#include "data.h"

class PHYSICS_TILE : public OBJECT_BASE
{
  public:
    PHYSICS_TILE(const TILE &t) : e(t.e), pos(t.pos)
    {
    }

    void logic()
    {
        pos.y += 1;
        if (pos.y > SCREEN_H)
            kill();
    }

    void render(BITMAP *buffer)
    {
        draw_sprite(buffer, (*data)[e], pos.x, pos.y);
    }


    ELEMENTS e;
    POSITION pos;
  private:

};

#endif
