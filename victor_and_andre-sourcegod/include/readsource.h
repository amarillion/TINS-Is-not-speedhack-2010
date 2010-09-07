/*
 * TINS 2010 - The source god
 * Authors: Victor Williams Stafusa da Silva
 *          André Luiz Pereira Álvares
 */
#ifndef __SENTINELA_READSOURCE
#define __SENTINELA_READSOURCE

#include <iostream>
#include <fstream>
#include <list>

class Character {
  private:
    char c;
    int x;
    int y;
    bool visible;

  public:
    Character(int x, int y, char c);
    virtual ~Character();

    int get_x();
    int get_y();
    char get_c();
    bool is_valid();
    bool is_visible();
    void set_visible(bool visible);
};

class CharacterIterator {
  private:
    std::list<std::list<Character*>*>* srccode;
    std::list<std::list<Character*>*>::iterator it_linhas;
    std::list<Character*>* linha;
    std::list<Character*>::iterator it_char;

  public:
    CharacterIterator(std::list<std::list<Character*>*>* srccode);
    bool has_next();
    Character *next();
};

std::list<std::list<Character*>*>* get_source_by_line(const char *file, const int linhainicial);

#endif /* __SENTINELA_READSOURCE */