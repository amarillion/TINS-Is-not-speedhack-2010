#include "data.h"

counted_ptr<DATA> data;


#include "datafile.h"


DATA::DATA() : data(NULL)
{
    data = load_datafile("data.dat");
    if (data == NULL)
        throw "Unable to load data. :(";
}
DATA::~DATA()
{
    if (data)
    {
        unload_datafile(data);
    }
}

BITMAP *DATA::operator[](ELEMENTS e)
{
    switch (e)
    {
      case WIND:
        return (BITMAP *)data[D_WIND].dat;
        break;
      case WATER:
        return (BITMAP *)data[D_WATER].dat;
        break;
      case EARTH:
        return (BITMAP *)data[D_EARTH].dat;
        break;
      case FIRE:
        return (BITMAP *)data[D_FIRE].dat;
        break;
      case HEART:
        return (BITMAP *)data[D_HEART].dat;
        break;
    }
    return NULL;
}
