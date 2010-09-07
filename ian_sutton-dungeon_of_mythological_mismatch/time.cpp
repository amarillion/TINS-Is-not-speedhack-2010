#include "time.hpp"
#include "logging.hpp"

volatile int Time::speed_counter = 0;
volatile int Time::game_ticks = 0;
int Time::ticks_per_sec = 0;


void IncSpeedCounter()
{
    ++Time::speed_counter;
    ++Time::game_ticks;
}
END_OF_FUNCTION(IncSpeedCounter)


bool Time::InitMainLoopTimer(int ticks_per_sec)
{
    srand(time(NULL));
    erlog("Initialising main game loop timing");
    if (Time::ticks_per_sec > 0) {
        erlog("Main game loop has been previously initialised", 1);
        return true;
    }
    if (install_timer() != 0) {
        erlog("Failed to install timers", 4);
        return false;
    }
    erlog("Timers installed");

    LOCK_VARIABLE(speed_counter);
    LOCK_VARIABLE(game_ticks);
    LOCK_FUNCTION(IncSpeedCounter);

    if (install_int_ex(IncSpeedCounter, BPS_TO_TIMER(ticks_per_sec)) != 0) {
        erlog("Failed to add loop timer", 4);
        return false;
    }
    Time::ticks_per_sec = ticks_per_sec;
    erlog("Loop timer added");
    return true;

}

bool Time::DoLogic()
{
    return speed_counter > 0;
}

std::string Time::Timestamp(std::string format)
{
    time_t rawtime;
    struct tm* timeinfo;
    char buffer[format.length()*4];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, 80, format.c_str(), timeinfo);
    return std::string(buffer);
}

