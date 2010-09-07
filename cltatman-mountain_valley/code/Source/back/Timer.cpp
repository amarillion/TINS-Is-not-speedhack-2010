#include "Timer.hpp"

#include <allegro.h>
#include <cstdio>


volatile unsigned long Timer::ticks;
volatile unsigned long Timer::time;


Timer::Timer( unsigned _frequency )
{
  frequency = _frequency;

  ticks =
  time =
  frames = 0;
}


void Timer::start()
{
  install_param_int_ex( Timer::tick, this, BPS_TO_TIMER( frequency ) );
}


void Timer::tick( void* that )
{
  Timer* t = (Timer*) that;

  t->ticks++;
  t->time++;
}


void Timer::wait()
{
  while( !doUpdate() )
    rest( 1 );
}


bool Timer::doUpdate()
{
  return ticks > 0;
}


void Timer::didUpdate()
{
  ticks--;
}


void Timer::drewFrame()
{
  frames++;
}


double Timer::fps()
{
  return double( frames ) / ( double(time) / double(frequency) );
}

