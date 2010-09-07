#include <Level.hpp>
#include <global/FileIO.hpp>

#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;


Level::Level( const string& t )
{
  title = t;
}


void Level::addBody( Body body )
{
  bodies.push_back( body );
}


void Level::addDecal( Frame decal )
{
  Animation animation( "default" );

  animation.addFrame( decal );

  decals.push_back( animation );
}


void Level::addDecal( Animation animation )
{
  decals.push_back( animation );
}


void Level::prepareDecals()
{
/*  ol::Vec2D pos;

  for( unsigned i = 0; i < decals.size(); i++ )
  {
    for( unsigned j = 0; j < decals[ i ].frames.size(); j++ )
    {
      Frame frame = decals[ i ].frames[ j ];

      pos = (frame._placement.GetPosition() / frame._z );

      decals[ i ].frames[ j ].setPlacement( pos.x + ol::Canvas::Width()/2,
                                            pos.y + ol::Canvas::Height()/2,
                                            frame._placement.GetRotation() );

      decals[ i ].frames[ j ]._scale /= frame._z;
    }
  }*/
}


void Level::update()
{
  for( unsigned i = 0; i < decals.size(); i++ )
    decals[ i ].update( 1 );

  roster.update();
}


void Level::save( const string& filename )
{
  ofstream file;

  file.open( filename.c_str() );

  file << "--Level 0"   << endl
       << title         << endl
       << bodies.size() << endl
       << deathBodies.size() << endl;

  for( unsigned i = 0; i < bodies.size(); i++ )
    bodies[ i ].save( file );

  for( unsigned i = 0; i < deathBodies.size(); i++ )
    deathBodies[ i ].save( file );

  file << decals.size() << endl;

  for( unsigned i = 0; i < decals.size(); i++ )
    decals[ i ].save( file );

  file.close();
}


void Level::load( const string& filename )
{
  ifstream file;

  file.open( filename.c_str() );

  if( !file.is_open() )
    return;

  if( getLine( file ) != "--Level 0" )
    return;

  title = getLine( file );

  int numberOfBodies = fromLine< int >( file );
  int numberOfDeathBodies = fromLine< int >( file );

  for( int i = 0; i < numberOfBodies; i++ )
  {
    Body body = Body( 0.0,0.0 );

    body.load( file );

    addBody( body );
  }

  for( int i = 0; i < numberOfDeathBodies; i++ )
  {
    Body body = Body( 0.0,0.0 );

    body.load( file );

    deathBodies.push_back( body );
  }

  int numberOfDecals = fromLine< int >( file );

  for( int i = 0; i < numberOfDecals; i++ )
  {
    Animation anim( "" );

    anim.load( file );

    addDecal( anim );
  }
}
