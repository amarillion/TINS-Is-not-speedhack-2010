#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include <allegro.h>

#include <vector>
#include <string>



class Keyboard
{
  public:
    Keyboard();

    void update();

    bool check( const std::string& keyState );

    bool operator[] ( const std::string& keyState ){ return check( keyState ); }

  private:
    int oldKeyStates[ KEY_MAX ];

    std::vector< std::string > keyNames;
    std::vector< std::string > keyEvents;
};

#endif // KEYBOARD_HPP
