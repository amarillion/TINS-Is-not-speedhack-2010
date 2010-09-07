#include <allegro.h>
#include <cmath>
#include "vector.hpp"
#include "logging.hpp"
#include "screen.hpp"
#include "time.hpp"
#include "animation.hpp"
#include "stages.hpp"
#include "colours.hpp"

const int scr_w = 640;
const int scr_h = 480;

int which_cycle = 0;
void GoToStage(Stage* which)
{
    stage = which;
    which_cycle = -1;
}

bool play_as_good = true;


int main(int argc, char* argv[])
{
    //erlog.LogToFile("Log.log");
    erlog("Initialising allegro");
    if (allegro_init() != 0) {
        erlog("Allegro could not be initialised", 4);
        return 0;
    }

    set_color_depth(32);

    Screen scr;
    if (!scr.SetPageFlipping(scr_w, scr_h, false) && !scr.SetDoubleBuffer(scr_w, scr_h, false))
        return 0;
    scr.SetTitle("Ian Sutton's TINS 2010 entry");

    if (install_keyboard() != 0) {
        erlog("Could not install keyboard", 4);
        return 0;
    }

    BITMAP* buffer; // Unasigned as it simply acts as a pointer to the Screen bitmap
    Colour::Init();

    // Load animation stuff
    if (!LoadGlobalDatafile())
        return 0;
    if (!Animation::LoadSequences())
        return 0;

    erlog("Making blood splatter");
    AddBloodSplatter(4);
    erlog("Making blood splatter");
    AddBloodSplatter(4);
    erlog("Making blood splatter");
    AddBloodSplatter(4);
    erlog("Making blood splatter");
    AddBloodSplatter(4);
    erlog("Making blood splatter");
    AddBloodSplatter(4);

    Time::InitMainLoopTimer(60);

    stage = &main_menu_stage;

    bool play = true;
    while (play) {
        while (!Time::DoLogic())
            rest(1);
        while (Time::DoLogic()) {
            /*if (key[KEY_ESC]) {
                play = false;
                erlog("Exited via escape key");
            }*/

            if (key[KEY_O]) erlog.SetAlpha(erlog.Alpha()+1);
            if (key[KEY_P]) erlog.SetAlpha(erlog.Alpha()-1);


            if (which_cycle <= 0) {
                if (!stage->FirstCycle()) {
                    play = false;
                    erlog("Exited from game on stage first cycle");
                }
                which_cycle = 1;
            }
            else {
                if (!stage->Cycle()) {
                    play = false;
                    erlog("Exited from game on stage general cycle");
                }
                ++which_cycle;
            }

            Time::DoneLogic();
        }
        if (which_cycle <= 1) continue;
        buffer = scr.GetCanvas();

        if (!stage->Draw(buffer)) {
            play = false;
            erlog("Exited from game during drawing", 3);
        }

        //erlog.Draw(buffer, 5, 5, 20);

        scr.ReleaseCanvas();
    }

    ReleaseGlobalDatafile();
    return 0;
}
END_OF_MAIN()
