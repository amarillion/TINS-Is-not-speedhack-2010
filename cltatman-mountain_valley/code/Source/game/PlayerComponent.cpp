#include <game/PlayerComponent.hpp>
#include <back/Game.hpp>



//REGISTER( PlayerComponent );


PlayerComponent::PlayerComponent() : Component( "PlayerComponent" )
{
  angelCounter = 200;
  xVel = yVel = 0.0;
  gravity = 0.2;
  speed = 2.5;
  angel = false;
  state = "idle";

  startX = 1717.5;
  startY = 732.5;

  windVol = 64.0;
  windPan = 128.0;
  windFreq = 300.0;

  transformCounter = 0;
  flapCounter = 0;
  deathCounter = -1;

  wind2 = load_sample( "data/sfx/wind.wav" );
  wind = load_sample( "data/sfx/wind.wav" );
  die = load_sample( "data/sfx/wind.wav" );
  play_sample( wind, windVol, windPan, windFreq, 1 );
  play_sample( wind2, windVol/1.5, 200, windFreq/2.13, 1 );
}


void PlayerComponent::update()
{
  if( game.level.roster.get( parent ).x < -840 )
  {
    if( game.level.roster.get( parent ).y < -259 )
      game.endTime = game.timer.time;
  }
  else if( game.level.roster.get( parent ).x > 3584 )
  {
    if( game.level.roster.get( parent ).y > 1980 )
      game.endTime = game.timer.time;
  }

  windVol += double( rand() % 400 ) / 100.0 - ( windVol > 50 ? 2.0 : 0.0 );
  windPan += double( rand() % 200 ) / 100.0 - 1.0;
  windFreq += double( rand() % 400 ) / 100.0 - 2.0;

  adjust_sample( wind, windVol, windPan, windFreq, 1 );

  if( deathCounter > 0 )
    deathCounter--;

  else if( transformCounter > 0 )
    transformCounter--;

  else
  {
    handleAnimation();
    handleControls();
  }

  if( deathCounter == -1 )
  {
    game.level.roster.get( parent ).y += yVel;
    game.level.roster.get( parent ).x += xVel;
  }

  for( int i = 0; i < game.level.deathBodies.size(); i++ )
    if( game.level.roster.get( parent ).body.
        testCollision( game.level.deathBodies[ i ] ).GetMagnitudeSquared() > 0.0001 &&
        deathCounter == -1 )
    {
      deathCounter = 60;
      play_sample( die, 255,128, 7000, 0 );
      game.level.roster.get( parent ).sprite.setAnimation( "die" );
    }

  if( deathCounter == 0 )
  {
    game.level.roster.get( parent ).x = startX;
    game.level.roster.get( parent ).y = startY;

    deathCounter = -1;
  }
}


void PlayerComponent::handleControls()
{
  if( angel )
  {
    game.isAngel = true;

    if( yVel > 0.0 && game.level.roster.get( parent ).collisions > 0 )
    {
      transformCounter = 10;
      angel = false;
    }
    else
      yVel += yVel < 4 ? gravity/3.0 : 0;

    if( flapCounter > 0 ) flapCounter--;

    if( game.keyboard[ "right held" ] && angelAngle < 0.5 ) angelAngle += 0.03;
    if( game.keyboard[ "left held"] && angelAngle > -0.5) angelAngle -= 0.03;

    if( game.keyboard[ "z pressed" ] && flapCounter == 0 )
    {
      double angelSpeed = 2;

      yVel -= sin( angelAngle + M_PI/2 ) * angelSpeed;
      xVel -= cos( angelAngle + M_PI/2 ) * (angelSpeed+1);

      if( yVel < -3 ) yVel = -3;
      if( xVel < -2 ) xVel = -2;
      if( xVel > 2 ) xVel = 2;

      flapCounter = 28;
    }

    game.angelAngle = angelAngle;
  }

  else
  {
    game.isAngel = false;

    angelAngle = 0;

    if( yVel > 0.0 && game.level.roster.get( parent ).collisions > 0 )
    {
      canJump = true;
      yVel = 0;
    }
    else
    {
      yVel += yVel < 6 ? gravity : 0;
    }
    if( !canJump )
    {
      game.level.roster.get( parent ).sprite.setAnimation( "jump" );
    }

    if( game.keyboard[ "left held" ] )
      xVel = -speed;
    else if( game.keyboard[ "right held" ] )
      xVel = speed;
    else
      xVel = 0;

    if( game.keyboard[ "z pressed" ] && canJump )
    {
      play_sample( die, 50, 128, 4000, 0 );
      yVel = -6;
      canJump = false;
      canAngel = true;
    }

    if( game.keyboard[ "x pressed" ] && canAngel )
    {
      angel = true;
      angelCounter = 200;
      transformCounter = 10;
      canAngel = false;
    }
  }
}


void PlayerComponent::handleAnimation()
{
  if( angel )
  {
    game.level.roster.get( parent ).sprite.setAnimation( "angel idle" );

    if( yVel > 0.0 && game.level.roster.get( parent ).collisions > 0 )
      game.level.roster.get( parent ).sprite.setAnimation( "to angel" );

    if( flapCounter > 0 )
      game.level.roster.get( parent ).sprite.setAnimation( "fly" );

    if( angelCounter-- == 0 )
    {
      angel = false;
      game.level.roster.get( parent ).sprite.setAnimation( "to human" );
      transformCounter = 10;
    }
  }


  else
  {
    game.level.roster.get( parent ).sprite.setAnimation( "idle" );

    if( game.keyboard[ "left held" ] )
      game.level.roster.get( parent ).sprite.setAnimation( "walk" );
    else if( game.keyboard[ "right held" ] )
      game.level.roster.get( parent ).sprite.setAnimation( "walk" );

    if( game.keyboard[ "up held" ] )
      game.level.roster.get( parent ).sprite.setAnimation( "jump" );
    else if( game.keyboard[ "down held" ] )
      game.level.roster.get( parent ).sprite.setAnimation( "die" );

    if( game.keyboard[ "x pressed" ] )
      game.level.roster.get( parent ).sprite.setAnimation( "to angel" );
  }
}
