#include "bejeweled_board.h"

#include <algorithm>

BEJEWELED_BOARD::BEJEWELED_BOARD(int w, int h) : BOARD(w, h)
{
}

// this is run after all the clicking and decision to move are made
void BEJEWELED_BOARD::logic(CP_OBJECT_MANAGER<TILE> &manager)
{

    std::list<counted_ptr<TILE> > nukable;
    POSITION to = selection;
    to.move(direction);

    GRID_DATA temp = data;
    std::swap(temp.at(selection), temp.at(to));
    int nukable_size = temp.find_nukable();
    if (nukable_size > 0)
    {
        std::swap(data.at(selection), data.at(to));
        data.shove_all();
    }
    else
    {
        counted_ptr<TILE> &one = temp.at(selection);
        counted_ptr<TILE> &two = temp.at(to);
        POSITION one_p, two_p;
        data.grid_to_screen(selection, one_p);
        data.grid_to_screen(to, two_p);
        one->shove(one_p, true);
        two->shove(two_p, true);
    }
}
