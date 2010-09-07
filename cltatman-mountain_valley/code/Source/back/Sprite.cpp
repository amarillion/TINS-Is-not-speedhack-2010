#include <Sprite.hpp>
#include <Renderer.hpp>
#include <global/FileIO.hpp>

#include <iostream>
#include <fstream>

using namespace std;


Sprite::Sprite()
{
  playSpeed = 1;
}


Sprite::Sprite( string filename )
{
  ifstream file( filename.c_str(), ifstream::in );

  load( file );

  playSpeed = 1;
}


void Sprite::addAnimation( Animation animation )
{
  animations[ animation.title ] = animation;
}


void Sprite::setAnimation( const string& title )
{
  currentAnimation = title;
}


void Sprite::setSpeed( double value )
{
  playSpeed = value;
}


void Sprite::update()
{
  if( animations.find( currentAnimation ) == animations.end() )
    currentAnimation = animations.begin()->first;

  animations[ currentAnimation ].update( playSpeed );
}


void Sprite::draw( double x, double y, double z, double rotation,
                   double xScale, double yScale, double opacity )
{
  Frame frame = animations[ currentAnimation ].getCurrentFrame();

  Frame temp = Frame( frame._imageFile, frame._time );

  temp.setPlacement( frame._placement.GetPosition().x + x,
                     frame._placement.GetPosition().y + y,
                     frame._placement.GetRotation() + rotation );
  temp.setScale( frame._scale.x * xScale,
                 frame._scale.y * yScale );

  temp._tint = frame._tint;

  temp._z = z;

  temp._opacity = frame._opacity * opacity;

//  Renderer::_()->add( temp ); THIS USES POINTERS.  NO GOOD.
}


Animation Sprite::getCurrentAnimation()
{
  return animations[ currentAnimation ];
}


void Sprite::save( ofstream& file )
{
  file << "--Sprite 0"      << endl
       << animations.size() << endl;

  map< string, Animation >::iterator it;

  for( it = animations.begin(); it != animations.end(); it++ )
    it->second.save( file );

  file.close();
}


void Sprite::load( ifstream& file )
{
  if( !file.is_open() )
    return;

  if( getLine( file ) != "--Sprite 0" )
    return;

  int numberOfAnimations = fromLine< int >( file );

  for( int i = 0; i < numberOfAnimations; i++ )
  {
    Animation animation( "" );
    animation.load( file );
    addAnimation( animation );
  }

  file.close();
}
