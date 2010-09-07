#ifndef OVERLAY_HPP
#define OVERLAY_HPP

#include <allegro.h>
#include <vector>

class Overlay {
    public:
        Overlay();

        bool Init();
        void Update();
        void Draw(BITMAP* buffer);

    protected:
        BITMAP* main_bmp;
        //std::vector<Animation> anims;
};



#endif
