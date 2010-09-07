#include "tile.h"

#include "movement.h"

#include <algorithm>

TILE::TILE(POSITION p, ELEMENTS el) : pos(p), e(el), nuked(false), start(pos), dir(NONE), returning(false)
{
}

TILE::TILE(POSITION p) : pos(p), nuked(false), start(pos), dir(NONE), returning(false)
{
    switch(rand() % 5)
    {
      case 0:
        e = WIND;
        break;
      case 1:
        e = WATER;
        break;
      case 2:
        e = EARTH;
        break;
      case 3:
        e = FIRE;
        break;
      case 4:
        e = HEART;
        break;
    }
}

void TILE::shove(POSITION dest, DIRECTION d, bool r)
{
    if (d == NONE)
        return;

    start = pos;

    destination = dest;
    dir = d;
    returning = r;

    update_xy(dir, POSITION(MAX_X, MAX_Y), pos);
}

void TILE::shove(POSITION dest, bool r)
{
    DIRECTION d;
    if (dest.x < pos.x)
    {
        d = W;
    }
    else if (dest.x > pos.x)
    {
        d = E;
    }
    else if (dest.y < pos.y)
    {
        d = N;
    }
    else if (dest.y > pos.y)
    {
        d = S;
    }
    else
        d = NONE;


    if (d == NONE)
        return;
    start = pos;

    destination = dest;
    dir = d;
    returning = r;

    update_xy(dir, POSITION(MAX_X, MAX_Y), pos);
}

void TILE::logic()
{
    if (!at_destination())
    {
        update_xy(dir, POSITION(MAX_X, MAX_Y), pos);
        if (at_destination())
        {
            if (returning)
            {
                std::swap(start, destination);
                returning = false;
                dir = invert(dir);
            }
            else
            {
                dir = NONE;
            }
        }
    }
    else
    {
        start = pos;
        dir = NONE;
        returning = false;
//            if (ungrid_request)
//                in_grid = false;

    }
}


bool TILE::will_arive(int distance)
{
    POSITION t = pos;
    for (int c = 0; c < distance;c++)
    {
        update_xy(dir, POSITION(MAX_X, MAX_Y), t);
        if ((t.x == destination.x) && (t.y == destination.y))
            return true;
    }
    return false;
}

#include "data.h"

void TILE::render(BITMAP *buffer)
{
    draw_sprite(buffer, (*data)[e], pos.x, pos.y);
}

bool TILE::at_destination()
{
    if (dir == NONE)
        return true;
    return ((pos.x == destination.x) && (pos.y == destination.y));
}
/*
void TILE::ungrid()
{
    ungrid_request = true;
}
*/
