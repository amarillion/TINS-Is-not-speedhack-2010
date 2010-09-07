#include <allegro.h>
#include "counted_ptr.h"
#include <algorithm>
#include "bejeweled_board.h"
#include "chuzzle_board.h"

#include "data.h"

#include "mouse_button.h"

volatile int logic_counter = 0;
void logic_counter_handler()
{
    logic_counter++;
}
END_OF_FUNCTION(logic_counter_handler)

#include "cp_object_manager.h"

int main()
{
    srand(time(NULL));
    allegro_init();
    install_keyboard();
    install_mouse();


    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 600, 600, 0, 0);
    show_mouse(screen);
    BITMAP *buffer = create_bitmap(SCREEN_W, SCREEN_H);


    CP_OBJECT_MANAGER<TILE> manager;
    CP_OBJECT_MANAGER<PHYSICS_TILE> pt_manager;

    data = counted_ptr<DATA>(new DATA);

    textout_ex(screen, font, "Press C for Chuzzle, any other key for bejeweled", 0, 0, makecol(255,255,255), -1);

    int k = readkey();
    BOARD *bjb;
    if ((k >> 8) == KEY_C)
        bjb = new CHUZZLE_BOARD(8,8);
    else
        bjb = new BEJEWELED_BOARD(8, 8);

    std::list<counted_ptr<TILE> > nuked;

    install_int(logic_counter_handler, 33);
    bool quitting = false;
    while (!quitting)
    {
        clear(buffer);

        while (logic_counter > 0)
        {
            for (int c = 0; c < 10; c++)
            {
                bjb->run(1, manager);
                pt_manager.logic();
                manager.logic();
                for (std::list<counted_ptr<TILE> >::iterator it = manager.items.begin(); it != manager.items.end(); it++)
                {
                    pt_manager.add(counted_ptr<PHYSICS_TILE>(new PHYSICS_TILE(**it)));
                    (*it)->kill();
                }
                pt_manager.prune();
                manager.prune();
            }

            logic_counter--;
        }

        if (left_mouse())
        {
            bjb->click(mouse_x, mouse_y);
        }

        bjb->render();
        draw_sprite(buffer, bjb->get_buffer(), 0, 0);
        manager.render(buffer);
        pt_manager.render(buffer);
        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

        if (key[KEY_ESC])
            quitting = true;
    }

    data = counted_ptr<DATA>(NULL);




    delete bjb;

    destroy_bitmap(buffer);
    show_mouse(NULL);
    return 0;
}
END_OF_MAIN()
