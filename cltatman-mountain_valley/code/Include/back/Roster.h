#ifndef ROSTER_H
#define ROSTER_H

#include <Message.h>
#include <Entity.h>
#include <Frame.hpp>

#include <string>
#include <map>


class Roster
{
  public:
    Roster();

    void add( Entity& entity );

    Entity& get( unsigned long id );
    Entity& get( const std::string& title );

    void broadcastMessage( Message message );

    void sendMessage( unsigned long id, Message message );
    void sendMessage( const std::string& title, Message message );

    void update();

    std::vector< Frame > getFrames();

    std::map< unsigned long, Entity > _entities;
    std::map< std::string, unsigned long > _titleLookup;
};

#endif // ROSTER_H
