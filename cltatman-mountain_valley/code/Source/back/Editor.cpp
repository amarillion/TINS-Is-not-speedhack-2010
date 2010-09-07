#include <back/Editor.hpp>

#include <Game.hpp>
#include <Frame.hpp>

#include <dirent.h>


static std::vector< std::string > getImageFilenames( const std::string& directory )
{
  std::vector< std::string > filenames;

  DIR* dir = opendir( directory.c_str() );

  struct dirent* ent;

  if( dir != NULL )
  {
    while( ( ent = readdir( dir ) ) != NULL )
    {
      std::string filename = ent->d_name;

      if( filename.find( ".png" ) != std::string::npos )
        filenames.push_back( ent->d_name );
    }

    closedir( dir );
  }

  return filenames;
}


Editor::Editor()
{
  enabled = false;
  gridSize = 8;
  cursorX = cursorY = 0;
  cursorZ = 1;
  font = ol::TextRenderer( "data/fonts/VeraMono.ttf", 12,12, ol::Rgba( 0.0,0.0,0.0,0.6 ) );
  mode = "idle";
  metaMode = "decal editing";
  imageStripSelection = 0;
}


void Editor::initialize()
{
  imageFiles = getImageFilenames( "data/gfx/" );

  for( unsigned i = 0; i < imageFiles.size(); i++ )
    images.push_back( Frame( "data/gfx/" + imageFiles[ i ] ) );

  cursorX = game.width/2;
  cursorY = game.height/2;
}


void Editor::update()
{
  if( game.keyboard[ "f1 released" ] ) game.level.save( "data/levels/world.lvl" );
  if( game.keyboard[ "f2 released" ] ) game.level.load( "data/levels/world.lvl" );

  if( metaMode == "decal editing" )
  {
    if( mode == "place decal" )
    {
      if( game.keyboard[ "q held" ] )
      {
        cursorX -= (int(cursorX)%int(gridSize/cursorZ));
        cursorY -= (int(cursorY)%int(gridSize/cursorZ));

        if( game.keyboard[ "left pressed" ] ) cursorX -= int(gridSize/cursorZ);
        if( game.keyboard[ "right pressed" ] ) cursorX += gridSize/cursorZ;
        if( game.keyboard[ "up pressed" ] ) cursorY -= gridSize/cursorZ;
        if( game.keyboard[ "down pressed" ] ) cursorY += gridSize/cursorZ;
      }
      else
      {
        if( game.keyboard[ "left held" ] ) cursorX--;
        if( game.keyboard[ "right held" ] ) cursorX++;
        if( game.keyboard[ "up held" ] ) cursorY--;
        if( game.keyboard[ "down held" ] ) cursorY++;
      }

      if( game.keyboard[ "a pressed" ] )
      {
        cursorZ -= 0.1;
        workingFrame.setScale( 1/cursorZ, 1/cursorZ );
      }
      if( game.keyboard[ "s pressed" ] )
      {
        cursorZ += 0.1;
        workingFrame.setScale( 1/cursorZ, 1/cursorZ );
      }
      if( game.keyboard[ "z pressed" ] )
        mode = "adjust decal";
      if( game.keyboard[ "x pressed" ] )
        mode = "idle";
    }

    else if( mode == "adjust decal" )
    {
      if( game.keyboard.check( "q held" ) )
      {
        if( game.keyboard.check( "left pressed" ) )
        {
          double x = workingFrame._placement.GetPosition().x,
                 y = workingFrame._placement.GetPosition().y,
                 r = workingFrame._placement.GetRotation() - ((M_PI*2)/360) * 45;
          workingFrame.setPlacement( x,y,r );
        }
        if( game.keyboard.check( "right pressed" ) )
        {
          double x = workingFrame._placement.GetPosition().x,
                 y = workingFrame._placement.GetPosition().y,
                 r = workingFrame._placement.GetRotation() + ((M_PI*2)/360) * 45;
          workingFrame.setPlacement( x,y,r );
        }
        if( game.keyboard.check( "up pressed" ) )
        {
          double sx = workingFrame._scale.x + 0.1,
                 sy = workingFrame._scale.y + 0.1;

          workingFrame.setScale( sx,sy );
        }
        if( game.keyboard.check( "down pressed" ) )
        {
          double sx = workingFrame._scale.x - 0.1,
                 sy = workingFrame._scale.y - 0.1;

          workingFrame.setScale( sx,sy );
        }
      }

      else
      {
        if( game.keyboard.check( "left held" ) )
        {
          double x = workingFrame._placement.GetPosition().x,
                 y = workingFrame._placement.GetPosition().y,
                 r = workingFrame._placement.GetRotation() - (M_PI*2)/360;
          workingFrame.setPlacement( x,y,r );
        }
        if( game.keyboard.check( "right held" ) )
        {
          double x = workingFrame._placement.GetPosition().x,
                 y = workingFrame._placement.GetPosition().y,
                 r = workingFrame._placement.GetRotation() + (M_PI*2)/360;
          workingFrame.setPlacement( x,y,r );
        }
        if( game.keyboard.check( "up held" ) )
        {
          double sx = workingFrame._scale.x + 0.1,
                 sy = workingFrame._scale.y + 0.1;

          workingFrame.setScale( sx,sy );
        }
        if( game.keyboard.check( "down held" ) )
        {
          double sx = workingFrame._scale.x - 0.1,
                 sy = workingFrame._scale.y - 0.1;

          workingFrame.setScale( sx,sy );
        }
      }

      if( game.keyboard.check( "a pressed" ) )
        workingFrame.setOpacity( workingFrame._opacity - 0.1 );
      if( game.keyboard.check( "s pressed" ) )
        workingFrame.setOpacity( workingFrame._opacity + 0.1 );
      if( game.keyboard.check( "x pressed" ) )
        mode = "place decal";
      if( game.keyboard.check( "z pressed" ) )
      {
        Frame frame = workingFrame;

        frame.setPlacement( floor(cursorX - game.width/2),
                            floor(cursorY - game.height/2),
                            frame._placement.GetRotation() );
        frame._z = cursorZ;
        frame.setScale( frame._scale.x*cursorZ,frame._scale.y*cursorZ );
        game.level.addDecal( frame );
        cursorZ = 1;

        mode = "idle";
      }
    }

    else if( mode == "idle" )
    {
      if( game.keyboard.check( "w pressed" ) )
        mode = "select image";

      if( game.keyboard.check( "leftShift pressed" ) )
        metaMode = "body editing";
    }

    else if( mode == "select image" )
    {
      if( game.keyboard.check( "left pressed" ) && imageStripSelection > 0 )
        imageStripSelection--;
      if( game.keyboard.check( "right pressed" ) && imageStripSelection < images.size()-1 )
        imageStripSelection++;

      if( game.keyboard.check( "w released" ) )
      {
        mode = "place decal";

        workingFrame = images[ imageStripSelection ];
        workingFrame.setScale( 1,1 );
      }
    }

    if( mode != "select image" && mode != "place decal" && mode != "adjust decal" )
    {
      if( game.keyboard.check( "left held" ) ) cursorX -= 4;
      if( game.keyboard.check( "right held" ) ) cursorX += 4;
      if( game.keyboard.check( "up held" ) ) cursorY -= 4;
      if( game.keyboard.check( "down held" ) ) cursorY += 4;
    }
  }

  else if( metaMode == "body editing" )
  {
    if( mode == "idle" )
    {
      if( game.keyboard.check( "leftShift pressed" ) )
        metaMode = "decal editing";

      if( game.keyboard.check( "x pressed" ) && workingPoints.size() > 2 )
      {
        Body body;

        body.points = workingPoints;
//        game.level.addBody( body );  //uncomment to fix.
        game.level.deathBodies.push_back( body );
        workingPoints.clear();
      }

      if( game.keyboard.check( "z pressed" ) )
      {
        bool existingPoint = false;

        for( int i = 0; i < workingPoints.size(); i++ )
          if( ( ol::Vec2D( cursorX, cursorY ) - workingPoints[ i ] ).GetMagnitude() < 10 )
          {
            workingPointsSelection = i;
            existingPoint = true;
            break;
          }

        if( !existingPoint )
        {
          ol::Vec2D point( cursorX,cursorY );
          workingPoints.push_back( point );
          if( workingPoints.empty() )
            workingPointsSelection = 0;
          else workingPointsSelection = workingPoints.size()-1;
        }

        mode = "moving point";
      }

      if( game.keyboard.check( "left held" ) ) cursorX -= 5;
      if( game.keyboard.check( "right held" ) ) cursorX += 5;
      if( game.keyboard.check( "up held" ) ) cursorY -= 5;
      if( game.keyboard.check( "down held" ) ) cursorY += 5;
    }

    else if( mode == "moving point" )
    {
      if( game.keyboard.check( "q held" ) )
      {
        cursorX = cursorX - (int(cursorX)%int(gridSize));
        cursorY = cursorY - (int(cursorY)%int(gridSize));

        if( game.keyboard.check( "left pressed" ) ) cursorX -= gridSize;
        if( game.keyboard.check( "right pressed" ) ) cursorX += gridSize;
        if( game.keyboard.check( "up pressed" ) ) cursorY -= gridSize;
        if( game.keyboard.check( "down pressed" ) ) cursorY += gridSize;

        workingPoints[ workingPointsSelection ].x = cursorX;
        workingPoints[ workingPointsSelection ].y = cursorY;
      }
      else
      {
        if( game.keyboard.check( "left held" ) ) cursorX -= 1;
        if( game.keyboard.check( "right held" ) ) cursorX += 1;
        if( game.keyboard.check( "up held" ) ) cursorY -= 1;
        if( game.keyboard.check( "down held" ) ) cursorY += 1;

        workingPoints[ workingPointsSelection ].x = cursorX;
        workingPoints[ workingPointsSelection ].y = cursorY;
      }

      if( game.keyboard.check( "w held" ) )
      {
        for( unsigned i = 0; i < game.level.bodies.size(); i++ )
        {
          std::vector< ol::Vec2D > points = game.level.bodies[ i ].points;

          for( unsigned j = 0; j < points.size(); j++ )
            if( ( points[ j ] - ol::Vec2D( cursorX,cursorY ) ).GetMagnitude() < 5 )
            {
              workingPoints[ workingPointsSelection ].x = points[ j ].x;
              workingPoints[ workingPointsSelection ].y = points[ j ].y;
            }
        }
      }

      if( game.keyboard.check( "z released" ) )
        mode = "idle";
    }
  }
}


static void drawImageStrip( std::vector< Frame > images, unsigned selection )
{
  double bX = 0,
         bY = ( game.height - ( ( game.height*2 ) / 5 ) ) / 2,
         bW = game.width,
         bH = (game.height *2) /5,
         margin = 20,
         imageSize = 100;

  ol::Rect( bX,bY, bW,bH ).Draw( ol::Rgba( 0.0,0.0,0.0,0.5 ) );

  for( unsigned i = 0; i < images.size(); i++ )
  {
    double cx = game.width/2 + ( int(i)-int(selection) )*( margin + imageSize ),
           cy = game.height/2;

    if( cx > 0 && cx < game.width )
    {
      double topLeftX = cx - ( imageSize/2 ),
             topLeftY = cy - ( imageSize/2 ),
             width = imageSize,
             height = width;

      ol::Rect( topLeftX,topLeftY, width,height ).Draw( ol::Rgba::WHITE );

      images[ i ].setPlacement( cx,cy );

      if( images[i]._width > images[i]._height && images[i]._width > imageSize )
      {
        double scale = imageSize / images[ i ]._width;

        images[ i ].setScale( scale,scale );
      }
      else if( images[i]._height > imageSize )
      {
        double scale = imageSize / images[ i ]._height;

        images[ i ].setScale( scale,scale );
      }

      images[ i ].draw();
    }
  }

  ol::Rect( game.width/2 - imageSize/2 - margin/2 + 2.5,
            game.height/2 - imageSize/2 - margin/2 + 2.5,
            imageSize + margin - 2.5, imageSize + margin - 2.5, 5,8 ).DrawOutline( ol::Rgba::YELLOW );
}


static void drawGrid( double cursorX, double cursorY, double z, unsigned size )
{
  size /= z;

  for( unsigned _x = 0 ; _x < game.width; _x += size )
  {
    double x = _x - ( int( game.camera.x ) % size );

    ol::Line( x,0, x,game.height ).Draw( ol::Rgba( 0.0,0.0,0.7,0.2 ) );
  }

  for( unsigned  _y = 0; _y < game.height; _y += size )
  {
    double y = _y - ( int( game.camera.y ) % size );

    ol::Line( 0,y, game.width,y ).Draw( ol::Rgba( 0.0,0.0,0.7,0.2 ) );
  }
}


void Editor::draw()
{
  font.Print( "Mode: " + mode + " | " + metaMode, 10,22 );

  if( mode == "idle" )
    ol::Circle( ol::Vec2D(cursorX - game.camera.x,cursorY - game.camera.y),
                5, 2, 10 ).DrawOutline( ol::Rgba( 0.0,0.0,0.7,0.6 ) );

  if( metaMode == "body editing" )
  {
    if( mode == "moving point" )
    {
      if( game.keyboard.check( "q held" ) )
      {
        drawGrid( cursorX, cursorY, cursorZ, gridSize );

        ol::Rect( cursorX - game.camera.x - (workingFrame._width/2)*workingFrame._scale.x,
                  cursorY - game.camera.y - (workingFrame._height/2)*workingFrame._scale.y,
                  workingFrame._width*workingFrame._scale.x,
                  workingFrame._height*workingFrame._scale.y ).DrawOutline( ol::Rgba::BLACK );
      }
    }

    for( unsigned i = 0; i < game.level.bodies.size(); i++ )
      game.level.bodies[ i ].draw();
    for( unsigned i = 0; i < game.level.deathBodies.size(); i++ )
      game.level.deathBodies[ i ].draw();

    if( !workingPoints.empty() && workingPoints.size() > 1 )
    {
      for( unsigned i = 0; i < workingPoints.size()-1; i++ )
      {
        double x1 = workingPoints[ i ].x - game.camera.x,
               y1 = workingPoints[ i ].y - game.camera.y,
               x2 = workingPoints[ i+1 ].x - game.camera.x,
               y2 = workingPoints[ i+1 ].y - game.camera.y;

        ol::Line( x1,y1, x2,y2 ).Draw( ol::Rgba( 1.0,0.0,1.0,0.5 ) );
      }

      ol::Line( workingPoints.back().x - game.camera.x, workingPoints.back().y - game.camera.y,
                workingPoints[ 0 ].x - game.camera.x, workingPoints[ 0 ].y - game.camera.y ).
                  Draw( ol::Rgba( 1.0,0.0,1.0,0.5 ) );
    }

    for( unsigned i = 0; i < workingPoints.size() ; i++ )
      if( ( ol::Vec2D( cursorX,cursorY ) - workingPoints[ i ] ).GetMagnitude() < 10 )
      {
        ol::Vec2D topLeft = workingPoints[ i ] -
                            ol::Vec2D( game.camera.x, game.camera.y ) -
                            ol::Vec2D( 5,5 );

        ol::Rect( topLeft, ol::Vec2D( 10,10 ) ).
                  DrawOutline( ol::Rgba( 0.5,0.0,0.5,0.5 ) );
      }
  }

  else if( mode == "place decal" || mode == "adjust decal" )
  {
    if( game.keyboard.check( "q held" ) )
    {
      drawGrid( cursorX, cursorY, cursorZ, gridSize );

      ol::Rect( cursorX - game.camera.x - (workingFrame._width/2)*workingFrame._scale.x,
                cursorY - game.camera.y - (workingFrame._height/2)*workingFrame._scale.y,
                workingFrame._width*workingFrame._scale.x,
                workingFrame._height*workingFrame._scale.y ).DrawOutline( ol::Rgba::BLACK );
    }

    workingFrame.setPlacement( cursorX - game.camera.x, cursorY - game.camera.y, workingFrame._placement.GetRotation() );
    workingFrame.draw();
  }

  if( mode == "select image" )
    drawImageStrip( images, imageStripSelection );
}
