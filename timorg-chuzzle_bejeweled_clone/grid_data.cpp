#include "grid_data.h"

#include <algorithm>

GRID_DATA::GRID_DATA(int w, int h) : _width(w), _height(h), data(NULL)
{
    data = new counted_ptr<TILE>[_height * _width];
}

GRID_DATA::GRID_DATA(const GRID_DATA &gd)
{
    if (this == &gd)
        return;

    _width = gd._width;
    _height = gd._height;
    data = new counted_ptr<TILE>[_height * _width];
    for (int c = 0; c < (_height * _width); c++)
    {
        data[c] = gd.data[c];
    }
}

GRID_DATA::~GRID_DATA()
{
    delete [] data;
}

counted_ptr<TILE> &GRID_DATA::at(POSITION p)
{
    return data[p.y * _width + p.x];
}

int GRID_DATA::width()
{
    return _width;
}

int GRID_DATA::height()
{
    return _height;
}

int GRID_DATA::find_nukable()
{
//    std::list<counted_ptr<TILE> > rval;
    int rval = 0;
    for(int y = 0; y < height(); y++)
    {
        for(int x = 0; x < width(); x++)
        {
            POSITION centre(x, y);

            // vertical
            if ((y > 0) && (y < (height() - 1)))
            {
                POSITION one(x, y - 1);
                POSITION two(x, y + 1);
                if ((at(one)->e == at(centre)->e) && (at(two)->e == at(centre)->e))
                {
                    at(one)->nuked = true;
                    at(centre)->nuked = true;
                    at(two)->nuked = true;
                }
            }
            // horizontal
            if ((x > 0) && (x < (width() - 1)))
            {
                POSITION one(x - 1, y);
                POSITION two(x + 1, y);
                if ((at(one)->e == at(centre)->e) && (at(two)->e == at(centre)->e))
                {
                    at(one)->nuked = true;
                    at(centre)->nuked = true;
                    at(two)->nuked = true;
                }
            }

        }
    }

    for(int y = 0; y < height(); y++)
    {
        for(int x = 0; x < width(); x++)
        {
            if (at(POSITION(x, y))->nuked)
            {
                rval++;
//                rval.push_back(at(POSITION(x, y)));
//                at(POSITION(x, y)) = counted_ptr<TILE>(NULL);
            }
        }
    }
    return rval; //remove_nukable();
}

std::list<counted_ptr<TILE> > GRID_DATA::remove_nukable()
{
    std::list<counted_ptr<TILE> > rval;

    for(int y = 0; y < height(); y++)
    {
        for(int x = 0; x < width(); x++)
        {
            if (at(POSITION(x, y))->nuked)
            {
//                rval++;
                rval.push_back(at(POSITION(x, y)));
                at(POSITION(x, y)) = counted_ptr<TILE>(NULL);
            }
        }
    }

    return rval;
}

void GRID_DATA::compact()
{
    for(int x = 0; x < width(); x++)
    {
        for(int z = 0; z < height(); z++)
        {
            for(int y = 1; y < height(); y++)
            {
                counted_ptr<TILE> &c = at(POSITION(x, y));
                counted_ptr<TILE> &p = at(POSITION(x, y - 1));
                if (c.get() == NULL)
                    std::swap(c, p);
            }
        }
    }
}

bool GRID_DATA::at_destinations()
{
    for(int y = 0; y < height(); y++)
    {
        for(int x = 0; x < width(); x++)
        {
            counted_ptr<TILE> &p = at(POSITION(x, y));
            if (p.get() != NULL)
            {
                if (!p->at_destination())
                {
//                    allegro_message("%d, %d", x, y);
                    return false;
                }
            }
        }
    }
    return true;
}

void GRID_DATA::fill_empty()
{
    for(int x = 0; x < width(); x++)
    {
        int c = 0;
        for(int y = (height() - 1); y >= 0; y--)
        {
            counted_ptr<TILE> &p = at(POSITION(x, y));
            if (p.get() == NULL)
            {
                POSITION dest;
                POSITION start;
                grid_to_screen(POSITION(x, -(c + 1)), start);
                grid_to_screen(POSITION(x, y), dest);
                p = counted_ptr<TILE>(new TILE(start));
                p->shove(dest, S, false);
                c++;
            }
        }
    }
}

void GRID_DATA::render(BITMAP *buffer, POSITION selection)
{
    POSITION end;
    grid_to_screen(POSITION(width() - 2, height() - 2), end);

    for(int y = 0; y < height(); y++)
    {
        for(int x = 0; x < width(); x++)
        {
            counted_ptr<TILE> &t = at(POSITION(x, y));
            if (t.get() != NULL)
            {
                t->render(buffer);

                if (t->pos.x > end.x)
                {
                    TILE tt(t->pos, t->e);
                    tt.pos.x -= MAX_X;
                    tt.render(buffer);
                }
                if (t->pos.y > end.y)
                {
                    TILE tt(t->pos, t->e);
                    tt.pos.y -= MAX_Y;
                    tt.render(buffer);
                }
            }
        }
    }

    POSITION p;
    grid_to_screen(selection, p);
    rect(buffer, p.x, p.y, p.x + TILE_W - 1, p.y + TILE_W - 1, makecol(255,255,255));

}

void GRID_DATA::grid_to_screen(POSITION grid, POSITION &m)
{
    m.x = grid.x * TILE_W;
    m.y = grid.y * TILE_W;
}
void GRID_DATA::screen_to_grid(POSITION m, POSITION &grid)
{
    grid.x = m.x / TILE_W;
    grid.y = m.y / TILE_W;
}

void GRID_DATA::logic()
{
    for(int y = 0; y < height(); y++)
    {
        for(int x = 0; x < width(); x++)
        {
            counted_ptr<TILE> &out = at(POSITION(x,y));
            if (out.get() != NULL)
                out->logic();
        }
    }
}

void GRID_DATA::shove_all()
{
    for(int y = 0; y < height(); y++)
    {
        for(int x = 0; x < width(); x++)
        {
            POSITION m;
            grid_to_screen(POSITION(x, y), m);
            counted_ptr<TILE> &out = at(POSITION(x,y));
            if (out.get() != NULL)
            {
                if (out->at_destination())
                    out->shove(m, false);
            }
        }
    }
}

std::list<counted_ptr<TILE> >  GRID_DATA::process()
{
    find_nukable();
    std::list<counted_ptr<TILE> > nukable = remove_nukable();
    compact();
    shove_all();
    fill_empty();
    return nukable;
}

void GRID_DATA::build_board()
{
    while(true)
    {
        process();
        logic();
        if (at_destinations())
            break;
    }
}

void GRID_DATA::rotate(int p, DIRECTION d)
{
    switch (d)
    {
      case NONE:
        break;
      case N:
        for(int y = 0; y < (height() - 1); y++)
        {
            std::swap(at(POSITION(p, y)), at(POSITION(p, y + 1)));
        }
        break;
      case E:
        for(int x = (width() - 1); x > 0; x--)
        {
            std::swap(at(POSITION(x, p)), at(POSITION(x - 1, p)));
        }
        break;
      case S:
        for(int y = (height() - 1); y > 0; y--)
        {
            std::swap(at(POSITION(p, y)), at(POSITION(p, y - 1)));
        }
        break;
      case W:
        for(int x = 0; x < (width() - 1); x++)
        {
            std::swap(at(POSITION(x, p)), at(POSITION(x + 1, p)));
        }
        break;
    };


}

void GRID_DATA::shove_line(int p, DIRECTION d)
{
    switch (d)
    {
      case NONE:
        break;
      case N:
      case S:
        for(int y = 0; y < height(); y++)
        {
            POSITION m;
            grid_to_screen(POSITION(p, y), m);
            counted_ptr<TILE> &out = at(POSITION(p,y));
            if (out.get() != NULL)
            {
                if (out->at_destination())
                    out->shove(m, d, false);
            }
        }
        break;
      case E:
      case W:
        for(int x = 0; x < width(); x++)
        {
            POSITION m;
            grid_to_screen(POSITION(x, p), m);
            counted_ptr<TILE> &out = at(POSITION(x,p));
            if (out.get() != NULL)
            {
                if (out->at_destination())
                    out->shove(m, d, false);
            }
        }
        break;
    };


}
