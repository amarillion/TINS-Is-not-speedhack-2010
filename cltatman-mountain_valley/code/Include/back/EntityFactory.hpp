#ifndef ENTITYFACTORY_HPP
#define ENTITYFACTORY_HPP


#include <back/Component.h>
#include <back/Entity.h>
#include <global/Components.hpp>

#include <string>
#include <map>


class EntityFactory
{
  public:
    EntityFactory();

    std::map< std::string, ComponentBuilder > componentRepo;
    std::map< std::string, std::string > entityRepo;

    void registerEntities();

    void addComponent( const std::string& type, ComponentBuilder );
    Component* getComponent( const std::string& type );
    Entity getEntity( const std::string& type );
};

#endif // ENTITYFACTORY_HPP
