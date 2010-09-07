#include "screen.hpp"
#include "logging.hpp"

Screen::Screen()
{
    erlog("Screen object initiated");
    buf[0] = NULL;
    buf[1] = NULL;
    which_buf = 0;
    draw_mode = -1;
}

Screen::~Screen()
{
    CleanUp();
    erlog("Screen object destroyed");
}

bool Screen::SetDoubleBuffer(int x, int y, bool fullscreen)
{
    erlog("Double buffer requested");
    CleanUp();
    if (set_gfx_mode(fullscreen?GFX_AUTODETECT_FULLSCREEN:GFX_AUTODETECT_WINDOWED, x, y, 0, 0) != 0) {
        erlog("Failed to initialise requested screen mode", 1);
        return false;
    }
    erlog("Requested screen mode initialised");

    buf[0] = create_bitmap(x, y);
    if (!buf[0]) {
        erlog("Failed to create screen buffer bitmap", 1);
        return false;
    }
    erlog("Screen buffer bitmap created");
    clear_bitmap(buf[0]);
    draw_mode = 0;
    erlog("Double buffer mode initialised");
    return true;
}

bool Screen::SetPageFlipping(int x, int y, bool fullscreen)
{
    erlog("Page flipping mode requested");
    CleanUp();
    if (set_gfx_mode(fullscreen?GFX_AUTODETECT_FULLSCREEN:GFX_AUTODETECT_WINDOWED, x, y, x*2, y) != 0) {
        erlog("Failed to initialise requested screen mode", 1);
        return false;
    }
    erlog("Requested screen mode initialised");

    buf[0] = create_video_bitmap(x, y);
    if (!buf[0]) {
        erlog("Failed to create first page", 1);
        return false;
    }
    buf[1] = create_video_bitmap(x, y);
    if (!buf[1]) {
        erlog("Failed to create second page", 1);
        destroy_bitmap(buf[0]);
        return false;
    }
    erlog("Video pages created");
    clear_bitmap(buf[0]);
    clear_bitmap(buf[1]);
    which_buf = 0;
    draw_mode = 1;
    erlog.Say("Page flipping mode initialised");
    return true;
}

BITMAP* Screen::GetCanvas()
{
    return buf[which_buf];
}

void Screen::ReleaseCanvas()
{
    if (draw_mode == 1) {
        show_video_bitmap(buf[which_buf]);
        which_buf = (which_buf+1) % 2;
        clear_bitmap(buf[which_buf]);
    }
    else if (draw_mode == 0) {
        blit(buf[0], screen, 0, 0, 0, 0, screen->w, screen->h);
        clear_bitmap(buf[0]);
    }
    else
        erlog("Unexpected screen drawing mode has been released");
}

void Screen::CleanUp()
{
    erlog("Cleaning up screen data");
    if (buf[0]) {
        destroy_bitmap(buf[0]);
        buf[0] = NULL;
    }
    if (buf[1]) {
        destroy_bitmap(buf[1]);
        buf[1] = NULL;
    }
    draw_mode = -1;
    which_buf = 0;
    erlog("Clean");
}
