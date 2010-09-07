#ifndef TIME_HPP
#define TIME_HPP

#include <string>

class Time {
    public:
        static bool InitMainLoopTimer(int ticks_per_sec);
        static bool DoLogic();
        static void DoneLogic() { --speed_counter; }

        static int Secs2Ticks(float secs) { return static_cast<int>(secs * ticks_per_sec + 0.5); }
        static float Ticks2Secs(int ticks) { return float(ticks) / float(ticks_per_sec); }

        static std::string Timestamp(std::string format);

        class Timer {
            public:
                Timer() { ticks_remaining = 0; }

                void SetTicks(int ticks) { ticks_remaining = ticks; }
                void SetSecs(float secs) { ticks_remaining = Secs2Ticks(secs); }

                bool Finished() { return (ticks_remaining == 0 ? true : false); }
                operator bool() { return (ticks_remaining == 0 ? true : false); }

                void Pause() { paused = true; }
                void UnPause() { paused = false; }
                void TogglePause() { paused = !paused; }

                void Update() { if (!paused && ticks_remaining > 0) --ticks_remaining; }

                int TicksRemaining() { return ticks_remaining; }
            protected:
                int ticks_remaining;
                bool paused;
        };

    protected:
        static volatile int speed_counter;
        static volatile int game_ticks;

        friend void IncSpeedCounter();

        static int ticks_per_sec;
};


#endif
