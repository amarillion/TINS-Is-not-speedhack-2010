#ifndef DATA_H
#define DATA_H

#include <allegro.h>
#include "elements.h"
#include "counted_ptr.h"

class DATA
{
  public:
    DATA();
    ~DATA();
    BITMAP *operator[](ELEMENTS e);
  private:
    DATAFILE *data;
};

extern counted_ptr<DATA> data;


#endif
