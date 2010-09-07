#include <Entity.h>
#include <Component.h>
#include <Game.hpp>
#include <global/FileIO.hpp>

using namespace std;


unsigned long Entity::_nextID = 0;


Entity::Entity()
{}


Entity::Entity( const string& _title = "", const string& _type = "" )
  : title( _title ), id( _nextID++ )
{
  hasBody = false;
  hasSprite = false;
}


int Entity::addComponent( Component* component )
{
  if( !component )
    return -1;

  if( components.find( component->type ) != components.end() )
    return -1;

  components[ component->type ] = component;

  component->parent = id;

  return 0;
}


int Entity::addTag( const string& tag )
{
  tags.push_back( tag );

  return 0;
}


int Entity::addMessage( Message& message )
{
  intermediateMessages.insert( pair< string, Message >( message.type, message ) );

  return 0;
}


Component* Entity::getComponent( const string& componentType )
{
  return components[ componentType ];
}


int Entity::removeComponent( const string& componentType )
{
  if( components.find( componentType ) == components.end() )
    return -1;

  components.erase( componentType );

  return 0;
}


bool Entity::hasComponent( const string& componentType )
{
  return ( components.find( componentType ) != components.end() );
}


bool Entity::hasMessage( const string& messageType )
{
  return messages.find( messageType ) != messages.end();
}


std::vector< Message > Entity::getMessages( const std::string& messageType )
{
  std::vector< Message > allMessages;

  std::multimap< std::string, Message >::iterator it = messages.begin();

  for( ; it != messages.end() ; it++ )
    if( it->first == messageType )
      allMessages.push_back( it->second );

  return allMessages;
}


void Entity::update()
{
  map< string, Component* >::iterator it = components.begin();

  messages = intermediateMessages;
  intermediateMessages.clear();

  for( ; it != components.end(); it++ )
    it->second->update();

  if( hasBody )
  {
    body.x = x;
    body.y = y;
  }

  sprite.update();
}


void Entity::save( ofstream& file )
{
  file << "--Entity 0" << endl
       << title        << endl
       << x            << endl
       << y            << endl
       << z            << endl
       << hasSprite    << endl;

  if( hasSprite )
    sprite.save( file );

  file << hasBody << endl;

  if( hasBody )
    body.save( file );

  file << tags.size() << endl;

  for( int i = 0 ; i < tags.size(); i++ )
    file << tags[ i ] << endl;

  file << messages.size() << endl;
  multimap< string, Message >::iterator it = messages.begin();
  for( ; it != messages.end(); it++ )
  {
    file << it->first << endl;
    it->second.save( file );
  }
  file << intermediateMessages.size() << endl;
  it = intermediateMessages.begin();
  for( ; it != intermediateMessages.end(); it++ )
  {
    file << it->first << endl;
    it->second.save( file );
  }
  file << components.size() << endl;
  map< string, Component* >::iterator i = components.begin();
  for( ; i != components.end(); i++ )
  {
    file << i->first << endl;
    i->second->save( file );
  }
}


void Entity::load( ifstream& file )
{
  if( !file.is_open() )
    return;
  if( getLine( file ) != "--Entity 0" )
    return;

  title = getLine( file );
  x = fromLine< double >( file );
  y = fromLine< double >( file );
  z = fromLine< double >( file );

  if( fromLine< bool >( file ) )
  {
    hasSprite = true;
    sprite.load( file );
  }

  if( fromLine< bool >( file ) )
  {
    hasBody = true;
    body.load( file );
  }

  unsigned tagSize = fromLine< unsigned >( file );

  for( int i = 0; i < tagSize; i++ )
    tags.push_back( getLine( file ) );

  unsigned messSize = fromLine< unsigned >( file );
  for( int i = 0; i < messSize; i++ )
  {
    string messageType = getLine( file );

    Message message;
    message.load( file );

    messages.insert( pair< string, Message >( messageType,message ) );
  }

  unsigned interSize = fromLine< unsigned >( file );
  for( int i = 0; i < interSize; i++ )
  {
    string messageType = getLine( file );

    Message message;
    message.load( file );

    intermediateMessages.insert( pair< string, Message >( messageType,message ) );
  }

  unsigned compSize = fromLine< unsigned >( file );
  for( int i = 0; i < compSize; i++ )
  {
    Component* component = game.entityFactory.getComponent( getLine( file ) );

    component->load( file );

    components[ component->type ] = component;
  }
}
