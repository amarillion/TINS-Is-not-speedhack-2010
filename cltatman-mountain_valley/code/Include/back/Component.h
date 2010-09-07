#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>


class Component
{
  public:
    Component( const std::string& _type );

    virtual ~Component(){};

    std::string   type;
    unsigned long parent;

    virtual void update(){};
    virtual void draw(){};
    virtual void load( std::ifstream& file ){};
    virtual void save( std::ofstream& file ){};
};

#endif // COMPONENT_H
