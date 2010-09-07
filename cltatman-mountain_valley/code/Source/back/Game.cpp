#include "Game.hpp"

#include <Renderer.hpp>
#include <Level.hpp>
#include <Timer.hpp>

extern void initGame();


Game::Game( unsigned w, unsigned h, const std::string& title, bool fullscreen )
{
  width = w;
  height = h;

  Renderer::_( w,h, fullscreen, title );

  install_sound( DIGI_AUTODETECT, MIDI_AUTODETECT, NULL );

  shouldQuit = false;

  keyboard = Keyboard();
  mouse = Mouse();

  level = Level( "level" );

  camera = Camera();
  endTime = 0;
}


void Game::initialize()
{
  editor.initialize();

  initGame();
}


void Game::run()
{
  initialize();

  timer.start();

  while( !shouldQuit )
  {
    timer.wait();

    while( timer.doUpdate() )
    {
      update();
      timer.didUpdate();
    }

    draw();
    timer.drewFrame();
  }
}

void Game::update()
{
  keyboard.update();
  mouse.update();

  if( keyboard[ "escape released" ] )
    shouldQuit = true;
//  if( keyboard[ "enter pressed" ] )
//    editor.enabled = !editor.enabled;

//  if( editor.enabled )
//    editor.update();
//  else level.update();
  if( endTime == 0 )
    level.update();

  camera.update();
}


ol::Bitmap edge( "data/gfx/edge.png" );
ol::Bitmap back( "data/gfx/pants.png" );
ol::TextRenderer billboard( "data/fonts/font.ttf", 25,25, ol::Rgba( 65,62,74 ) );
ol::TextRenderer smallBill( "data/fonts/font.ttf", 70,70, ol::Rgba( 247,228,190 ) );

void Game::drawBill( const std::string& text, double x, double y )
{
  x -= camera.x;
  y -= camera.y;

  if( x < game.width && x + 300 > 0 && y-50 < game.height && y + 50 > 0 )
    billboard.Print( text, x,y );
}


void Game::draw()
{
  back.BlitStretched( 0,0, game.width, game.height );

  camera.render( level );

  if( editor.enabled )
    editor.draw();
  else
    edge.BlitStretched( 0,0, game.width, game.height );

  if( endTime > 0 )
  {
    ol::Rect( 0,0, game.width,game.height ).Draw( ol::Rgba( 255,255,255, timer.time - endTime ) );

    if( timer.time - endTime > 500 )
      shouldQuit = true;
  }

  drawBill( "Press z, then x.", 1640,700 );
  drawBill( "Won't you go to our son?\nHe's lost on the mountain.", 1420,600 );
  drawBill( "You cannot go, they will die!\nI can hear my children crying.", 495,-15 );
  drawBill( "Please, they will drown.\nThe flood is coming.", -720,530 );
  drawBill( "The family drowned together.", -750,-255 );
  drawBill( "Your son is safe.", -940,-408 );

  drawBill( "Please, you must help!\nThey have fallen into the valley!", 1808,653 );
  drawBill( "Where are you going?\nWould you abandon us?", 2075,1070 );
  drawBill( "How can you do this?\nHe is our child!", 2790,1673 );
  drawBill( "Your son was killed\non the mountain.", 3305, 1993 );
  drawBill( "You saved the family.", 3628, 1925 );

  if( timer.time < 510 )
  {
    ol::Rect( 0,0, width,height ).Draw( ol::Rgba( 65,62,74, 255 - timer.time ) );
    smallBill.Print( "Mountain Valley", 50,game.height-60 );

    smallBill.SetColor( ol::Rgba( 247,228,190, 255 - timer.time/2 ) );
  }

  ol::Canvas::Refresh();
}
