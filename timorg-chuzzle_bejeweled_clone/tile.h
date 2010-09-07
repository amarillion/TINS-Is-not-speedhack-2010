#ifndef TILE_H
#define TILE_H

#include "object_base.h"

#include "direction.h"

#define MAX_X   (TILE_W * 8)
#define MAX_Y   (TILE_W * 8)

#define TILE_W  40

#include "elements.h"
#include "position.h"

class TILE : public OBJECT_BASE
{
  public:
    TILE(POSITION p, ELEMENTS el);
    TILE(POSITION p);
    void shove(POSITION dest, DIRECTION d, bool r);
    void shove(POSITION dest, bool r);

    virtual void logic();
    bool will_arive(int distance);
    virtual void render(BITMAP *buffer);
    bool at_destination();


    POSITION pos;
    ELEMENTS e;
    bool nuked;
    POSITION start;
    POSITION destination;
    DIRECTION dir;
    bool returning;

// request an ungrid
//    void ungrid();
//    bool ungrid_request;
//    bool in_grid;

  private:

};




#endif
