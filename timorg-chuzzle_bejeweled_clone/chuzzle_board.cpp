#include "chuzzle_board.h"

CHUZZLE_BOARD::CHUZZLE_BOARD(int w, int h) : BOARD(w, h)
{
}

// this is run after all the clicking and decision to move are made
void CHUZZLE_BOARD::logic(CP_OBJECT_MANAGER<TILE> &manager)
{
    std::list<counted_ptr<TILE> > nukable;
    GRID_DATA temp = data;

    int rotate_count = 0;

    bool no_nukable = true;

    if ((direction == N) || (direction == S))
    {
        for (int c = 0; c < data.height(); c++)
        {
            rotate_count = c;
            temp.rotate(selection.x, direction);
            //nukable =

            int nukable_size = temp.find_nukable();
//            nukable = temp.remove_nukable();
            if (nukable_size > 0)
            {
/*
                // add all nuked to the manager
                for (std::list<counted_ptr<TILE> >::iterator i = nukable.begin(); i != nukable.end(); i++)
                {
//                    (*i)->ungrid();
                    manager.add(counted_ptr<TILE>(*i));
                }
*/
                no_nukable = false;
                break;
            }
        }
        if (!no_nukable)
        {
            rotate_count++;
            for (int c = 0; c < rotate_count; c++)
            {
                data.rotate(selection.x, direction);
            }
            data.shove_line(selection.x, direction);
            data.shove_all();
        }
    }
    else if ((direction == E) ||(direction == W))
    {
        for (int c = 0; c < data.width(); c++)
        {
            rotate_count = c;
            temp.rotate(selection.y, direction);
            //nukable =
            int nukable_size = temp.find_nukable();
//            nukable = temp.remove_nukable();
            if (nukable_size > 0)
            {
/*
                // add all nuked to the manager
                for (std::list<counted_ptr<TILE> >::iterator i = nukable.begin(); i != nukable.end(); i++)
                {
//                    (*i)->ungrid();
                    manager.add(counted_ptr<TILE>(*i));
                }
*/
                no_nukable = false;
                break;
            }
        }
        if (!no_nukable)
        {
            rotate_count++;
            for (int c = 0; c < rotate_count; c++)
            {
                data.rotate(selection.y, direction);
            }
            data.shove_line(selection.y, direction);
            data.shove_all();
        }
    }

    if ((direction == N) || (direction == S))
    {
        for(int y = 0; y < data.height(); y++)
        {
            counted_ptr<TILE> &t  = data.at(POSITION(selection.x, y));
            if (no_nukable)
                t->shove(t->pos, direction, false);
        }
    }
    else if ((direction == E) ||(direction == W))
    {
        for(int x = 0; x < data.width(); x++)
        {
            counted_ptr<TILE> &t  = data.at(POSITION(x, selection.y));
            if (no_nukable)
                t->shove(t->pos, direction, false);
        }
    }
}
