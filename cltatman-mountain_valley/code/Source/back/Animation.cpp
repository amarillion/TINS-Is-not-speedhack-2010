#include <Animation.hpp>
#include <Frame.hpp>
#include <global/FileIO.hpp>

#include <string>
#include <vector>
#include <iostream>

using namespace std;


Animation::Animation(const string& _title )
  :title( _title ), time( 0.0 ), current( 0 )
{}


Animation::Animation( const string& _title, const string& query, double ticks,
                      double x, double y, double rotation, double xScale,
                      double yScale, double r, double g, double b, double a,
                      double opacity )
  :title( _title ), time( 0.0 ), current( 0 )
{
  struct al_ffblk file;
  string directory = query.substr( 0, query.find('*') );

  if( al_findfirst( query.c_str(), &file, FA_ALL ) == 0 )
  {
    Frame frame = Frame( directory + file.name, ticks );

    frame.setPlacement( x,y, rotation );
    frame.setScale( xScale,yScale );
    frame.setTint( r,g,b,a );
    frame.setOpacity( opacity );

    frames.push_back( frame );

    while( al_findnext( &file ) == 0 )
    {
      frame = Frame( directory + file.name, ticks );

      frame.setPlacement( x,y, rotation );
      frame.setScale( xScale,yScale );
      frame.setTint( r,g,b,a );
      frame.setOpacity( opacity );

      frames.push_back( frame );
    }
  }

  al_findclose( &file );
}


void Animation::addFrame( Frame frame )
{
  frames.push_back( frame );
}


void Animation::update( double _t )
{
  if( frames.empty() )
    return;

  time += _t;

  if( time > frames[current]._time )
  {
    if( current == frames.size()-1 ) current = 0;
    else current++;

    time = 0;
  }

  else if( time < 0 )
  {
    if( current == 0 ) current = frames.size()-1;
    else current--;

    time = frames[current]._time;
  }
}


Frame Animation::getCurrentFrame() const
{
  if( current < frames.size() )
    return frames[ current ];

  else return frames[ 0 ];
}


void Animation::setRotation( double rotation )
{
  for( int i = 0; i < frames.size(); i++ )
    frames[ i ]._placement.SetRotation( rotation );
}


void Animation::save( std::ofstream& file )
{
  file << "--Animation 0" << endl
       << title           << endl
       << frames.size()   << endl;

  for( unsigned i = 0; i < frames.size(); i++ )
    frames[ i ].save( file );
}


void Animation::load( std::ifstream& file )
{
  if( !file.is_open() )
    return;

  if( getLine( file ) != "--Animation 0" )
    return;

  title = getLine( file );

  int numberOfFrames = fromLine< int >( file );

  for( int j = 0; j < numberOfFrames; j++ )
  {
    Frame frame( "", 0 );

    frame.load( file );

    addFrame( frame );
  }
}
