#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <allegro.h>
#include <string>

class Screen {
    public:
        Screen();
        ~Screen();

        bool SetDoubleBuffer(int x, int y, bool fullscreen = false);
        bool SetPageFlipping(int x, int y, bool fullscreen = false);

        BITMAP* GetCanvas();
        void ReleaseCanvas();

        void CleanUp();
        void SetTitle(std::string title) { set_window_title(title.c_str()); }

    protected:
        BITMAP* buf[2];
        int which_buf;
        int draw_mode;
};

extern const int scr_w;
extern const int scr_h;

#endif
