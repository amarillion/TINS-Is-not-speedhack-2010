#include <Message.h>
#include <global/FileIO.hpp>

using namespace std;


Message::Message( const string& _type )
  : type( _type )
{}


void Message::save( ofstream& file )
{
  file << "--Message 0"     << endl
       << type              << endl
       << targetTags.size() << endl;

  for( int i = 0; i < targetTags.size(); i++ )
    file << targetTags[ i ] << endl;

  file << variables.size() << endl;

  map< string,string >::iterator it = variables.begin();

  for( ; it != variables.end(); it++ )
    file << it->first  << endl
         << it->second << endl;
}


void Message::load( ifstream& file )
{
  if( !file.is_open() )
    return;
  if( getLine( file ) != "--Message 0" )
    return;

  type = getLine( file );

  int tags = fromLine< int >( file );

  for( int i = 0; i < tags; i++ )
    addTag( getLine( file ) );

  int vars = fromLine< int >( file );

  for( int i = 0; i < vars; i++ )
    variables[ getLine( file ) ] = getLine( file );
}

