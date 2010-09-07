#include "Camera.hpp"
#include <Game.hpp>

#include <algorithm>


Camera::Camera()
{
  x = 0;
  y = 0;

  target = 0xFFFFFFFF;
}


static bool compareDepth( Frame a, Frame b )
{
  return a._z > b._z;
}

void Camera::render( Level level )
{
  std::vector< Frame > decals;

  ol::Vec2D pos;

  for( unsigned i = 0; i < level.decals.size(); i++ )
  {
    Frame frame = level.decals[ i ].getCurrentFrame();

    pos = ( frame._placement.GetPosition() - ol::Vec2D( x,y ) ) / frame._z;

    pos.x += game.width  / 2;
    pos.y += game.height / 2;

    ol::Vec2D scale = frame._scale / frame._z;

    double radius =
      ol::Vec2D((frame._width*scale.x)/2,(frame._height*scale.y)/2).GetMagnitude();

    double left =   pos.x + radius,
           right =  pos.x - radius,
           top =    pos.y - radius,
           bottom = pos.y + radius;
    if( left > 0 && bottom > 0 && right < game.width && top < game.height )
    {
      decals.push_back( frame );

      decals.back().setPlacement( pos.x, pos.y, decals.back()._placement.GetRotation() );

      decals.back()._scale = scale;
    }
  }

  std::vector< Frame > entityFrames = game.level.roster.getFrames();

  for( unsigned i = 0; i < entityFrames.size(); i++ )
  {
    pos = ( entityFrames[ i ]._placement.GetPosition() - ol::Vec2D( x,y ) ) /
            entityFrames[ i ]._z;

    double _x = entityFrames[ i ]._placement.GetPosition().x - x,
           _y = entityFrames[ i ]._placement.GetPosition().y - y;

    entityFrames[ i ].setPlacement( _x,
                                    _y,
                                    entityFrames[ i ]._placement.GetRotation() );
  }

  decals.insert( decals.end(), entityFrames.begin(), entityFrames.end() );

  std::multimap< double, Component* > entityDraws;

  for( unsigned i = 0; i < game.level.roster._entities.size(); i++ )
  {
    double entZ = game.level.roster._entities[ i ].z;

    std::map< std::string, Component* >::iterator it =
      game.level.roster._entities[i].components.begin();

    for( ; it != game.level.roster._entities[i].components.end(); it++ )
      entityDraws.insert( std::pair< double, Component* >( entZ, it->second ) );
  }

  std::stable_sort( decals.begin(), decals.end(), compareDepth );

  std::multimap< double, Component* >::iterator lb,ub;

  for( unsigned i = 0; i < decals.size(); i++ )
  {
    lb = entityDraws.lower_bound( decals[ i ]._z );
    if( i > 0 )
      ub = entityDraws.lower_bound( decals[i-1]._z );
    else
      ub = entityDraws.lower_bound( 1000.0 ); // Arbitrary upper-bound for Z.

    for( ; lb != ub; lb++ )
      lb->second->draw();

    decals[ i ].draw();
  }
}


void Camera::update()
{
  double tx = game.width/2 - x,
         ty = game.height/2 - y;

  if( game.editor.enabled )
  {
    tx = game.editor.cursorX - x;
    ty = game.editor.cursorY - y;
  }

  else if( target != 0xFFFFFFFF )
  {
    Entity _target = game.level.roster.get( target );

    tx = _target.x - x;
    ty = _target.y - y;
  }

  double sw = game.width,
         sh = game.height;

  if( tx < sw/2 )
    x -= ( sw/2 - tx ) / 60.0;
  if( tx > sw - ( sw/2 ) )
    x += ( sw/2 - ( sw - tx ) ) / 60.0;

  if( ty < sh/2 )
    y -= ( sh/2 - ty ) / 60.0;
  if( ty > sh - ( sh/2 ) )
    y += ( sh/2 - ( sh - ty ) ) / 60.0;
}
