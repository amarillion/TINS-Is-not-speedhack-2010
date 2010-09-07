#ifndef BOARD_H
#define BOARD_H

#include <allegro.h>

#include "position.h"
#include "grid_data.h"

#include "cp_object_manager.h"

#include "physics_tile.h"

class BOARD
{
  public:
    BOARD(int w, int h);
    virtual ~BOARD();


    void click(int x, int y);
    int stage();
    void clear_stage();
    int dir();
    void render();

    void run(int count, CP_OBJECT_MANAGER<TILE> &manager);

    GRID_DATA data;
    POSITION selection;
    DIRECTION direction;

    BITMAP *get_buffer();

  private:
    virtual void logic(CP_OBJECT_MANAGER<TILE> &manager) = 0;
    int click_stage;
    BITMAP *b;
};

#endif

