#ifndef TIMER_HPP
#define TIMER_HPP


class Timer
{
  public:
    Timer( unsigned _frequency = 60 );

    static void tick( void* that );
    void start();
    void wait();
    bool doUpdate();
    void didUpdate();
    void drewFrame();
    double fps();

    unsigned frequency;
    static volatile unsigned long time,
                                  ticks;
    unsigned long frames;
};

#endif // TIMER_HPP

