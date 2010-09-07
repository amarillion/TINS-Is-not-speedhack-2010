#include "board.h"

BOARD::BOARD(int w, int h) : data(w, h), selection(-1, -1), direction(NONE), click_stage(0), b(NULL)
{
    b = create_bitmap(MAX_X, MAX_Y);
    if (b == NULL)
        throw "unable to create BOARD backbuffer";
    for(int y = 0; y < data.height(); y++)
    {
        for(int x = 0; x < data.width(); x++)
        {
            POSITION p;
            data.grid_to_screen(POSITION(x,y), p);
            data.at(POSITION(x,y)) = counted_ptr<TILE>(new TILE(p));
        }
    }
    data.build_board();
}

BOARD::~BOARD()
{
    if (b != NULL)
        destroy_bitmap(b);
}



void BOARD::click(int x, int y)
{
    if (click_stage == 0)
    {
        data.screen_to_grid(POSITION(x, y), selection);
        click_stage = 1;
    }
    else if (click_stage == 1)
    {
        POSITION temp;
        data.screen_to_grid(POSITION(x, y), temp);
        if ((temp.x != selection.x) && (temp.y != selection.y))
        {
            clear_stage();
            click(x, y);
            return;
        }
        if (temp.x < selection.x)
            direction = W;
        if (temp.x > selection.x)
            direction = E;
        if (temp.y < selection.y)
            direction = N;
        if (temp.y > selection.y)
            direction = S;
        click_stage = 2;
    }
}

int BOARD::stage()
{
    return click_stage;
}

void BOARD::clear_stage()
{
    click_stage = 0;
}

int BOARD::dir()
{
    if (click_stage < 2)
        return NONE;
    return direction;
}


void BOARD::render()
{
    clear(b);
    data.render(b, selection);
}

void BOARD::run(int count, CP_OBJECT_MANAGER<TILE> &manager)
{
    for (int c = 0; c < count; c++)
    {
        if (stage() == 1)
        {
            if (!data.at_destinations())
            {
                clear_stage();
            }

        }
        else if (stage() == 2)
        {
            logic(manager);
            selection = POSITION(-1, -1);
            clear_stage();
        }

        data.logic();


        if (data.at_destinations())
        {
            std::list<counted_ptr<TILE> > nukable = data.process();
            for (std::list<counted_ptr<TILE> >::iterator i = nukable.begin(); i != nukable.end(); i++)
            {
//                    (*i)->ungrid();
                manager.add(counted_ptr<TILE>(*i));
            }

        }
    }
}

BITMAP *BOARD::get_buffer()
{
    return b;
}
