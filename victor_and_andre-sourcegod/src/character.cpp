/*
 * TINS 2010 - The source god
 * Authors: Victor Williams Stafusa da Silva
 *          André Luiz Pereira Álvares
 */
#include "readsource.h"

// Classe Character

Character::Character(int x, int y, char c) {
    this->x = x;
    this->y = y;
    this->c = c;
    this->visible = is_valid();
}

Character::~Character() {
}

int Character::get_x() {
    return x;
}

int Character::get_y() {
    return y;
}

char Character::get_c() {
    return c;
}

bool Character::is_valid() {
    return c != ' ' && c != '\n' && c != '\t';
}

void Character::set_visible(bool visible) {
    this->visible = visible;
}

bool Character::is_visible() {
    return visible;
}

// Classe CharacterIterator

CharacterIterator::CharacterIterator(std::list<std::list<Character*>*>* srccode) {
    this->srccode = srccode;
    this->it_linhas = srccode->begin();
    this->linha = (it_linhas == srccode->end() ? NULL : *it_linhas);
    if (linha != NULL) this->it_char = linha->begin();
}

bool CharacterIterator::has_next() {
    return it_linhas != srccode->end();
}

Character *CharacterIterator::next() {
    if (!has_next()) throw "";

    Character *ret = *it_char;
    *it_char++;
    if (it_char != linha->end()) return ret;

    while (true) {
        it_linhas++;
        if (it_linhas == srccode->end()) return ret;
        linha = *it_linhas;
        it_char = linha->begin();
        if (it_char != linha->end()) return ret;
    }
}