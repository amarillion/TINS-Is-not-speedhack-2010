#include "Keyboard.hpp"

#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;



static vector< string > doKeyNames( const string& names )
{
  stringstream ss( names );
  string s;

  vector< string > v;

  while( getline( ss, s, ' ' ) )
    v.push_back( s );

  return v;
}


Keyboard::Keyboard()
{
  for( int i = 0; i < KEY_MAX; i++ )
    oldKeyStates[i] = key[i];

  keyNames = doKeyNames( "* a b c d e f g h i j k l m n o p q r s t u v w x y z 0 1 2 3 4 5 6 7 8 9 pad0 pad1 pad2 pad3 pad4 pad5 pad6 pad7 pad8 pad9 f1 f2 f3 f4 f5 f6 f7 f8 f9 f10 f11 f12 escape tilde minus equals backspace tab openBrace closeBrace enter colon quote backslash backslash2 comma stop slash space insert delete home end pageUp pageDown left right up down padSlash asterisk padMinus padPlus padDelete padEnter printScreen pause ABNT_C1 yen kana convert noConvert at circumflex colon2 kanji * * semicolon * * * * * * * * * leftShift rightShift leftControl rightControl leftAlt rightAlt leftWin rightWin menu scrollLock numLock capsLock padEquals backQuote semicolon command" );
}


void Keyboard::update()
{
  keyEvents.clear();

  for( int i = 0; i < KEY_MAX; i++ )
  {
    if( key[ i ] && oldKeyStates[ i ] )
      keyEvents.push_back( keyNames[ i ] + " held" );

    else if( key[ i ] && !oldKeyStates[ i ] )
      keyEvents.push_back( keyNames[ i ] + " pressed" );

    else if( !key[ i ] && oldKeyStates[ i ] )
      keyEvents.push_back( keyNames[ i ] + " released" );

    oldKeyStates[ i ] = key[ i ];
  }
}


bool Keyboard::check( const string& keyState )
{
  return find( keyEvents.begin(), keyEvents.end(), keyState ) != keyEvents.end();
}
