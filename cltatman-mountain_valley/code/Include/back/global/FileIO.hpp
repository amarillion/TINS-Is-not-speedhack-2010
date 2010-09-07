#ifndef FILEIO_HPP
#define FILEIO_HPP


#include <string>
#include <sstream>
#include <fstream>


template< typename T >
inline T fromLine( std::ifstream& file )
{
  T t;
  std::string line;

  getline( file, line );

  std::istringstream iss( line );
  iss >> t;

  return t;
}


inline std::string getLine( std::ifstream& file )
{
  std::string line;

  getline( file, line );

  return line;
}

#endif
