#include "Mouse.hpp"

#include <allegro.h>

#include <algorithm>
#include <iostream>
#include <vector>
#include <string>


Mouse::Mouse()
{
  oldX = mouse_x;
  oldY = mouse_y;
  oldZ = mouse_z;
  oldState = mouse_b;
}


void Mouse::update()
{
  buttonStates.clear();

  if( ( mouse_b & 1 ) && ( oldState & 1 ) )
    buttonStates.push_back( "leftButton held" );
  else if( ( mouse_b & 1 ) && !( oldState & 1 ) )
    buttonStates.push_back( "leftButton pressed" );
  else if( !( mouse_b & 1 ) && ( oldState & 1 ) )
    buttonStates.push_back( "leftButton released" );

  if( ( mouse_b & 2 ) && ( oldState & 2 ) )
    buttonStates.push_back( "rightButton held" );
  else if( ( mouse_b & 2 ) && !( oldState & 2 ) )
    buttonStates.push_back( "rightButton pressed" );
  else if( !( mouse_b & 2 ) && ( oldState & 2 ) )
    buttonStates.push_back( "rightButton released" );

  if( ( mouse_b & 4 ) && ( oldState & 4 ) )
    buttonStates.push_back( "middleButton held" );
  else if( ( mouse_b & 4 ) && !( oldState & 4 ) )
    buttonStates.push_back( "middleButton pressed" );
  else if( !( mouse_b & 4 ) && ( oldState & 4 ) )
    buttonStates.push_back( "middleButton released" );

  if( mouse_z > oldZ )
    buttonStates.push_back( "wheel up" );
  else if( mouse_z < oldZ )
    buttonStates.push_back( "wheel down" );

  oldX = mouse_x;
  oldY = mouse_y;
  oldZ = mouse_z;
  oldState = mouse_b;
}


void Mouse::getPosition( double& x, double& y )
{
  x = mouse_x;
  y = mouse_y;
}


void Mouse::getMovement( double& x, double& y )
{
  x = mouse_x - oldX;
  y = mouse_y - oldY;
}


bool Mouse::check( const std::string& state )
{
  return std::find( buttonStates.begin(), buttonStates.end(), state ) != buttonStates.end();
}
