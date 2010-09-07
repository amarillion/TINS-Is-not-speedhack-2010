#include <allegro.h>

#include <back/Game.hpp>


Game game( 640,400, "Mountain Valley", false );


int main( int argc, char** argv )
{
  game.run();

  return 0;
}
END_OF_MAIN()
