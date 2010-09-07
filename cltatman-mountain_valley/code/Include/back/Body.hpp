#ifndef BODY_HPP
#define BODY_HPP

#include <vector>
#include <OpenLayer.hpp>


class Body
{
  public:
    Body( double _x = 0, double _y = 0 );

    void addPoint( double _x, double _y );
    void insertPoint( double _x, double _y );
    void removePoint();

    void draw();

    void updateBounds();

    ol::Vec2D testCollision( Body body );
    std::vector< ol::Vec2D > absolutePoints();

    void save( std::ofstream& file );
    void load( std::ifstream& file );

    double x,y, // center
           top,
           bottom,
           left,
           right;

    std::vector< ol::Vec2D > points;
};


#endif
