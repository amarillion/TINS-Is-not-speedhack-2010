#include "mouse_button.h"

#include <allegro.h>

MOUSE_BUTTON left_mouse(1);

MOUSE_BUTTON::MOUSE_BUTTON(int b) : button(b), button_down(false)
{
}
bool MOUSE_BUTTON::operator()()
{
    if (mouse_b & button)
    {
        if (!button_down)
        {
            button_down = true;
            return true;
        }
    }
    else
        button_down = false;
    return false;
}
