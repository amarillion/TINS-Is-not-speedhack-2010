#ifndef GRID_DATA_H
#define GRID_DATA_H

#include "position.h"
#include "counted_ptr.h"
#include "tile.h"

#include <list>

class GRID_DATA
{
  public:
    GRID_DATA(int w, int h);
    GRID_DATA(const GRID_DATA &gd);
    ~GRID_DATA();
    counted_ptr<TILE> &at(POSITION p);
    int width();
    int height();

    std::list<counted_ptr<TILE> > remove_nukable();
    int find_nukable();
    void compact();

    void render(BITMAP *buffer, POSITION selection);


    void fill_empty();

    bool at_destinations();

    void grid_to_screen(POSITION grid, POSITION &m);
    void screen_to_grid(POSITION m, POSITION &grid);

    void logic();

    void shove_all();

    std::list<counted_ptr<TILE> >  process();

    void build_board();

    void rotate(int p, DIRECTION d);

    void shove_line(int p, DIRECTION d);

  private:
    int _width, _height;
    counted_ptr<TILE> *data;
};

#endif
