#ifndef MESSAGE_H
#define MESSAGE_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>


class Message
{
  public:
    Message( const std::string& _type = "default" );

    std::string type; // Hit, explode, etc.

    std::vector< std::string > targetTags; // Target must have ALL these tags.

    std::map< std::string, std::string > variables;

    void addTag( const std::string& tag )
    {
      targetTags.push_back( tag );
    }

    template< class T >
    void set( const std::string& name, T value )
    {
      std::stringstream ss;

      ss << value;

      variables[ name ] = ss.str();
    }

    template< class T >
    T get( const std::string& variable )
    {
      T t;

      if( variables.find( variable ) != variables.end() )
      {
        std::istringstream iss( variables[ variable ] );

        iss >> t;

        return t;
      }

      else return 0.0;
    }

    void save( std::ofstream& file );
    void load( std::ifstream& file );
};

#endif // MESSAGE_H
