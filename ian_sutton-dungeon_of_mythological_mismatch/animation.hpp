#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <allegro.h>
#include <list>
#include <map>
#include <string>
#include <cmath>
#include "vector.hpp"
#include "time.hpp"
#include "datafile.hpp"
#include "colours.hpp"

extern DATAFILE* global_datafile;
bool LoadGlobalDatafile();
void ReleaseGlobalDatafile();

void CrayonLine(BITMAP* bmp, Vector start, Vector end, int colour = Colour::Carbon(), float wobble = 0.02, float amplitude = 1, int inner_radius = 0, int outer_radius = 1, float hardness = 0.5);

void CrayonTally(BITMAP* bmp, Vector top_left, int number, int colour = Colour::Carbon(), int height = 20);

BITMAP* GetBloodSplatter();
void AddBloodSplatter(int how_many);
void BloodSplatter(BITMAP* bmp, Vector pos, int colour1 = Colour::BloodRed1(), int colour2 = Colour::BloodRed2(), int colour3 = Colour::BloodRed3());


class AnimFrame {
    public:
        AnimFrame();
        AnimFrame(int graphic_id,int ticks);
        ~AnimFrame();

        void Set(int id, int ticks) { graphic_id = id; this->ticks = ticks; }
        void SetGraphicId(int id) { graphic_id = id; }
        void SetTicks(int ticks) { this->ticks = ticks; }
        void SetSecs(float secs) { this->ticks = Time::Secs2Ticks(abs(secs)); }

        void Draw(BITMAP* bmp, Vector pos, float scale = 1.0, bool vertical_flip = false, float rotation = 0.0, int alpha = 256);
        // At present drawing with alpha will cancel out scale, v_flip and rotation. This is mostly because I'm lazy.
        // If its needed I'll fix it later.

        int GraphicId() { return graphic_id; }
        int Ticks() { return ticks; }

        static int HowMany() { return how_many; }

    protected:
        int graphic_id;
        int ticks;

        static int how_many;
};

class AnimSequence { // Basically just a list of frames
    public:
        AnimSequence() { ++how_many; }
        ~AnimSequence() { --how_many; }

        void Clear() { frame.clear(); }
        void Replace(std::list<AnimFrame>::iterator begin, std::list<AnimFrame>::iterator end) { frame.assign(begin, end); }
        void AppendFrame(AnimFrame f) { frame.push_back(f); }

        std::list<AnimFrame>::iterator Start() { return frame.begin(); }
        std::list<AnimFrame>::iterator End() { return frame.end(); }

        static int HowMany() { return how_many; }
    protected:
        std::list<AnimFrame> frame;

        static int how_many;
};


class Animation {
    public:
        Animation();
        Animation(AnimSequence*);
        Animation(std::string);

        void SetSequence(AnimSequence*);
        void SetSequence(std::string);

        void Tick();
        void Draw(BITMAP* bmp, Vector pos, float scale = 0.0, bool vertical_flip = false, float rotation = 0.0, int alpha = 256);

        bool ReachedEndAtLeastOnce();
        void SetLooping(bool looping) { this->looping = looping; } // Default looping on.

        void SetSpeed(float ticks) { ticks_per_game_tick = ticks>0.0?ticks:0.0; }; // How many animation ticks should be done for each game tick. Default 1
        float GetSpeed() { return ticks_per_game_tick; }

        void Pause();
        void UnPause();
        void TogglePause();
        void Restart();
        bool AtEnd();

        static bool LoadSequences();

        static int HowMany() { return how_many; }

    protected:
        AnimSequence* seq;

        std::list<AnimFrame>::iterator iter;
        std::list<AnimFrame>::iterator end;

        bool running;
        float frame_ticks_remaining;
        bool looping;
        float ticks_per_game_tick;

        int how_many_loops;

        bool use_glob_seq;
        std::string glob_seq_name;

        static std::map<std::string, AnimSequence> sequences;
        static bool sequences_loaded;

        static int how_many;
};



#endif
