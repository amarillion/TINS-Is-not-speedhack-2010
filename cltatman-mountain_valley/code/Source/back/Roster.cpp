#include <Roster.h>

#include <Entity.h>
#include <Game.hpp>
//#include <game/Components/Visual.hpp>
//#include <game/Components/Position.hpp>
//#include <game/Components/Collision.hpp>

#include <algorithm>



Roster::Roster()
{
  _entities.clear();
}


void Roster::add( Entity& entity )
{
  _entities[ entity.id ] = entity;
  _titleLookup[ entity.title ] = entity.id;
}


Entity& Roster::get( unsigned long id )
{
  return _entities[ id ];
}


Entity& Roster::get( const std::string& title )
{
  return _entities[ _titleLookup[ title ] ];
}


static bool hasTags( Entity entity, Message message )
{
  for( unsigned i = 0; i < message.targetTags.size(); i++ )
    if( std::find( entity.tags.begin(), entity.tags.end(),
                   message.targetTags[ i ] ) == entity.tags.end() )
      return false;

  return true;
}


void Roster::broadcastMessage( Message message )
{
  std::map< unsigned long, Entity >::iterator it;

  for( it = _entities.begin(); it != _entities.end(); it++ )
    if( hasTags( it->second, message ) )
      it->second.addMessage( message );
}


void Roster::sendMessage( unsigned long id, Message message )
{
  get( id ).addMessage( message );
}


void Roster::sendMessage( const std::string& title, Message message )
{
  get( title ).addMessage( message );
}


void Roster::update()
{
  std::map< unsigned long, Entity >::iterator it = _entities.begin();

  for( ; it != _entities.end(); it++ )
  {
    it->second.update();

    if( it->second.hasBody )
    {
      it->second.collisions = 0;

      for( unsigned i = 0; i < game.level.bodies.size(); i++ )
      {
        ol::Vec2D dis = it->second.body.testCollision( game.level.bodies[ i ] );

//        if( fabs( dis.GetMagnitudeSquared() ) > 0.001 )
//          it->second.collisions++;

        it->second.x -= dis.x;
        it->second.y -= dis.y;

        if( dis.y > 0 && dis.y > dis.x )
          it->second.collisions = 1;
      }
    }
  }
}


std::vector< Frame > Roster::getFrames()
{
  std::vector< Frame > frames;

  std::map< unsigned long, Entity >::iterator it = _entities.begin();

  for( ; it != _entities.end(); it++ )
  {
    if( it->second.hasSprite )
    {
      Frame frame = it->second.sprite.getCurrentAnimation().getCurrentFrame();
      double x = frame._placement.GetPosition().x + it->second.x,
             y = frame._placement.GetPosition().y + it->second.y,
             z = frame._z + it->second.z,
             rotation = frame._placement.GetRotation();

      if( game.isAngel )
        rotation += game.angelAngle;

      frame.setPlacement( x,y, rotation );
      frame._z = z;

      frames.push_back( frame );
    }
  }

  return frames;
}
