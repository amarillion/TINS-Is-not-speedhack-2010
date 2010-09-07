#include <back/Game.hpp>

#include <game/PlayerComponent.hpp>
#include <game/DustParticleComponent.hpp>


void initGame()
{
  Entity player( "player", "player" );

  player.x = 1717.5;
  player.y = 732.5;

  player.addComponent( new PlayerComponent() );

  Sprite sprite;

  std::ifstream spriteIn( "data/entities/sprites/player.spr" );
    sprite.load( spriteIn );
  spriteIn.close();

  player.sprite = sprite;
  player.hasSprite = true;

  Body playerBody( player.x, player.y );

  playerBody.addPoint( -12,-12 );
  playerBody.addPoint( 12,-12 );
  playerBody.addPoint( 12,12 );
  playerBody.addPoint( -12,12 );

  player.body = playerBody;
  player.hasBody = true;

  game.level.roster.add( player );

  game.camera.target = player.id;

  game.level.load( "data/levels/world.lvl" );

  for( int i = 0; i < 50; i++ )
  {
    Entity dust( "dust", "dust" + i );

    dust.addComponent( new DustParticleComponent() );

    Sprite dustSprite;

    Animation dustAnim( "default" );
    if( rand() % 2 == 0 )
      dustAnim.addFrame( Frame( "data/gfx/dust2.png", 1 ) );
    else
      dustAnim.addFrame( Frame( "data/gfx/dust1.png", 1 ) );
    dustSprite.addAnimation( dustAnim );

    dustSprite.setAnimation( "default" );

    dust.sprite = dustSprite;
    dust.hasSprite = true;

    game.level.roster.add( dust );
  }
}
