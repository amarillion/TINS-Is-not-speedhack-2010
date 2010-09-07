#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <Level.hpp>



class Camera
{
  public:
    Camera();

    void render( Level level );
    void update();

    double x,y;
    unsigned long target;
};

#endif // CAMERA_HPP
