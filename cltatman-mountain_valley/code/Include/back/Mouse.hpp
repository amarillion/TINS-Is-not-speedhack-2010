#ifndef MOUSE_HPP
#define MOUSE_HPP

#include <vector>
#include <string>


class Mouse
{
  public:
    Mouse();

    void update();

    void getPosition( double& x, double& y );
    void getMovement( double& x, double& y );

    bool check( const std::string& state );

    std::vector< std::string > buttonStates;

    int oldX,oldY,oldZ,
        oldState;
};

#endif // MOUSE_HPP
