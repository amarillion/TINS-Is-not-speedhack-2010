#pragma once

class Time
{
public:
	static const int TicksPerSecond = 60;
	
	static void init();
	static void reset();
	static long getTick();

private:
	Time();
	static void callback();
	static volatile long myCurrentTick;
};

