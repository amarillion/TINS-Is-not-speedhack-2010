#ifndef CHUZZLE_BOARD_H
#define CHUZZLE_BOARD_H

#include "board.h"

class CHUZZLE_BOARD : public BOARD
{
  public:
    CHUZZLE_BOARD(int w, int h);
  private:
    // this is run after all the clicking and decision to move are made
    virtual void logic(CP_OBJECT_MANAGER<TILE> &removed);
};

#endif
