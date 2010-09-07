#ifndef PLAYERCOMPONENT_HPP
#define PLAYERCOMPONENT_HPP

#include <Component.h>
#include <allegro.h>


class PlayerComponent : public Component
{
  public:
    PlayerComponent();
    virtual ~PlayerComponent(){};

    void update();
    void handleAnimation();
    void handleControls();

    bool angel,
         canJump,
         canAngel;
    unsigned angelCounter,
             transformCounter,
             flapCounter;
    int deathCounter;
    double startX,
           startY,
           xVel,
           yVel,
           speed,
           gravity,
           angelAngle;
    std::string state;

    SAMPLE* wind, * wind2, *die;

    double windVol,
           windPan,
           windFreq;
};

#endif // PLAYERCOMPONENT_HPP
