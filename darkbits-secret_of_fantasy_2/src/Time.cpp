#include "Precompiled.hpp"
#include "Time.hpp"

long volatile Time::myCurrentTick = 0;

Time::Time()
{
}

void Time::init()
{
	install_timer();
	LOCK_VARIABLE(myCurrentTick);
	LOCK_FUNCTION(callback);
	install_int_ex(callback, BPS_TO_TIMER(Time::TicksPerSecond));
}

void Time::reset()
{
	myCurrentTick = 0;
}

long Time::getTick()
{
	return myCurrentTick;
}

void Time::callback()
{
	++myCurrentTick;
}
END_OF_FUNCTION(callback)

