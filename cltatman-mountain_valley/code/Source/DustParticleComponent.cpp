#include <game/DustParticleComponent.hpp>
#include <Component.h>
#include <Game.hpp>



DustParticleComponent::DustParticleComponent() : Component( "DustParticleComponent" )
{
  setup = false;

  angle = -2.3;
  speed = double( rand() % 400 ) / 100.0 + 0.5;

  entered = false;
}


void DustParticleComponent::update()
{
  if( !setup )
  {
    double startX,startY;



    if( rand() % 2 == 1 )
    {
      double a = game.camera.x - 20,
             b = game.camera.x + game.width;

      startX = double( rand() % int( b - a ) ) + a;

      a = game.camera.y - 20;
      b = game.camera.y;

      startY = double( rand() % int( b - a ) ) + a;
    }

    else
    {
      double a = game.camera.x - 20,
             b = game.camera.x;

      startX = double( rand() % int( b - a ) ) + a;

      a = game.camera.y;
      b = game.camera.y + game.height;

      startY = double( rand() % int( b - a ) ) + a;
    }

    game.level.roster.get( parent ).x = startX;
    game.level.roster.get( parent ).y = startY;
    game.level.roster.get( parent ).z = double( rand() % 500 ) / 100.0 + 0.1;

    setup = true;
  }

  game.level.roster.get( parent ).x -= cos( angle ) * speed;
  game.level.roster.get( parent ).y -= sin( angle ) * speed;

  double parentX = game.level.roster.get( parent ).x,
         parentY = game.level.roster.get( parent ).y;

  if( !entered )
    if( parentX > 0 && parentX < game.camera.x + game.width &&
        parentY > 0 && parentY < game.camera.y + game.height )
      entered = true;

  if( entered )
  {
    if( parentX > ( game.camera.x + game.width ) )
      game.level.roster.get( parent ).x -= game.width;
    else if( parentX < game.camera.x )
      game.level.roster.get( parent ).x += game.width;
    if( parentY > ( game.camera.y + game.height ) )
      game.level.roster.get( parent ).y -= game.height;
    else if( parentY < game.camera.y )
      game.level.roster.get( parent ).y += game.height;
  }


  if( angle < -1.8 ) angle += double( rand() % 100 ) / 1500.0;
  if( angle > -2.3 ) angle -= double( rand() % 100 ) / 1500.0;

  if( speed < 5.0 ) speed += double( rand() % 100 ) / 3000.0;
  if( speed > 1.0 ) speed -= double( rand() % 100 ) / 3000.0;
}
