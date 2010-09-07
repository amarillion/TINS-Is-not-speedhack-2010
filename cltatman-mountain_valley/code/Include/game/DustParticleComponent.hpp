#ifndef DUSTPARTICLECOMPONENT_HPP
#define DUSTPARTICLECOMPONENT_HPP

#include <Component.h>


class DustParticleComponent : public Component
{
  public:
    DustParticleComponent();

    void update();

    bool setup,
         entered;

    double angle,
           speed;
};

#endif // DUSTPARTICLECOMPONENT_HPP
