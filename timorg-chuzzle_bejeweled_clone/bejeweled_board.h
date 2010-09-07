#ifndef BEJEWELED_BOARD_H
#define BEJEWELED_BOARD_H

#include "board.h"

class BEJEWELED_BOARD : public BOARD
{
  public:
    BEJEWELED_BOARD(int w, int h);
  private:
    // this is run after all the clicking and decision to move are made
    virtual void logic(CP_OBJECT_MANAGER<TILE> &removed);
};



#endif
