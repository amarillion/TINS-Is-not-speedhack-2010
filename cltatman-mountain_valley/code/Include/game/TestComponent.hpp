#ifndef TESTCOMPONENT_HPP
#define TESTCOMPONENT_HPP

#include <back/Component.h>


class TestComponent : public Component
{
  public:
    TestComponent();
    virtual ~TestComponent(){};

    void update(){}; // Any logic.
    void draw(){}; // Any drawing.

    void save( std::ofstream& file );
    void load( std::ifstream& file );
};

#endif // TESTCOMPONENT_HPP
