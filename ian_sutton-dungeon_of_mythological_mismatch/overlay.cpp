#include "overlay.hpp"
#include "animation.hpp"
#include "screen.hpp"
#include "colours.hpp"

Overlay::Overlay()
{
    main_bmp = NULL;
}

bool Overlay::Init()
{
    main_bmp = create_bitmap(scr_w, scr_h);
    if (!main_bmp)
        return false;
    clear_to_color(main_bmp, Colour::White());
    CrayonLine(main_bmp, Vector(10, 5), Vector(8, scr_h-44), Colour::Red(), 0.01, 2, 1, 2); // Vertical left
    CrayonLine(main_bmp, Vector(3, 9), Vector(scr_w-5, 7), Colour::Red(), 0.008, 2, 1, 2); // Horizontal top
    CrayonLine(main_bmp, Vector(scr_w-8, 6), Vector(scr_w-12, scr_h-50), Colour::Red(), 0.007, 2, 1, 2); // Vertical right
    CrayonLine(main_bmp, Vector(5, scr_h-55), Vector(scr_w-4, scr_h-53), Colour::Red(), 0.005, 2, 1, 2); // Horizontal bottom
    floodfill(main_bmp, 100, 100, Colour::MagicPink());

    return true;
}

void Overlay::Draw(BITMAP* bmp)
{
    draw_sprite(bmp, main_bmp, 0, 0);
}
