#ifndef OBJECT_BASE_H
#define OBJECT_BASE_H

#include <allegro.h>

class OBJECT_BASE
{
  public:
    OBJECT_BASE();
    bool is_alive();
    void kill();

    virtual void logic() = 0;
    virtual void render (BITMAP *buffer) = 0;

  private:
    bool alive;
};

#endif
