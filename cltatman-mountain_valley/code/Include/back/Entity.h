#ifndef ENTITY_H
#define ENTITY_H

#include <Message.h>
#include <Sprite.hpp>
#include <Body.hpp>

#include <string>
#include <vector>
#include <map>

class Component;


class Entity
{
  public:
    Entity();
    Entity( const std::string& _type, const std::string& _title );

    unsigned long id;

    std::string title;

    double x,y,z;

    int collisions;

    Sprite sprite;
    Body body;
    bool hasSprite,
         hasBody;

    std::vector< std::string > tags;
    std::multimap< std::string, Message > messages;
    std::multimap< std::string, Message > intermediateMessages;

    std::map< std::string, Component* > components;

    int  addComponent       ( Component* component );
    int  addTag             ( const std::string& tag );
    int  addMessage         ( Message& message );
    Component* getComponent ( const std::string& componentType );
    int  removeComponent    ( const std::string& componentType );
    bool hasComponent       ( const std::string& componentType );
    bool hasMessage         ( const std::string& messageType );

    std::vector< Message > getMessages( const std::string& messageType );

    void update();

    void save( std::ofstream& file );
    void load( std::ifstream& file );

  private:
    static unsigned long _nextID;
};

#endif // ENTITY_H
