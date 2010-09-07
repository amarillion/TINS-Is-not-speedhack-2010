/*
 * TINS 2010 - The source god
 * Authors: Victor Williams Stafusa da Silva
 *          André Luiz Pereira Álvares
 */
#include "game.h"

Fase::Fase(const char *file, const int linhainicial, const char *midi, int maos, int sprays, float codigo_ok, FaseStart *logic) {
    this->logic = logic;
    this->sprays = sprays;
    this->maos = maos;
    this->codigo_ok = codigo_ok;
    this->srccode = get_source_by_line(file, linhainicial);
    this->linhainicial = linhainicial;
    this->music = load_midi(midi);

    int count = 0;
    CharacterIterator *ci = iterate_source();
    while (ci->has_next()) {
        Character *character = ci->next();
        if (character->is_valid() && character->is_visible()) count++;
    }
    delete ci;

    this->char_count = count;
    this->total_char = count;
}

void Fase::inc_char_count() {
    this->char_count++;
}

void Fase::dec_char_count() {
    this->char_count--;
}

Fase::~Fase() {
    delete srccode;
    delete music;
    delete logic;
}

bool Fase::tick() {
    return logic->start(jogo);
}

void Fase::start(Jogo *jogo) {
    this->jogo = jogo;
    stop_midi();
    debug("babbaba");
    play_midi(this->music, -1);
    logic->start(jogo);
}

CharacterIterator *Fase::iterate_source() {
    return new CharacterIterator(srccode);
}

int Fase::get_sprays() {
    return sprays;
}

int Fase::get_maos() {
    return maos;
}

bool Fase::is_codigo_ok() {
    return ((float) char_count / (float) total_char) > codigo_ok;
}

float Fase::get_codigo_qtd() {
    return ((float) char_count / (float) total_char);
}

float Fase::get_codigo_ok() {
    return codigo_ok;
}