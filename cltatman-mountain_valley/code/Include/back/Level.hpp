#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <Animation.hpp>
#include <Frame.hpp>
#include <Body.hpp>
#include <Roster.h>

#include <string>


class Level
{
  public:
    std::string title;

    Level( const std::string& t = "level" );

    void addBody( Body body );
    void addDecal( Frame frame );
    void addDecal( Animation animation );

    void prepareDecals();

    void update();

    void save( const std::string& filename );
    void load( const std::string& filename );

    Roster roster;

    std::vector< Animation > decals;
    std::vector< Body > bodies;
    std::vector< Body > deathBodies;
};

#endif
