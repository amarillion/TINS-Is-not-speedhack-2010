/*
 * TINS 2010 - The source god
 * Authors: Victor Williams Stafusa da Silva
 *          André Luiz Pereira Álvares
 */
#include "game.h"

// Classe Pedra.

#define PEDRA_LARGURA 40
#define PEDRA_ALTURA 40

Pedra::Pedra(int x, int y, Jogo *jogo) :
        Personagem(TIPO_PEDRA, x, y, PEDRA_LARGURA, PEDRA_ALTURA, 1, 1, jogo) {
    this->sprite_offset_x = -3;
    this->sprite_offset_y = 0;
}

Pedra::~Pedra() {
}

BITMAP *Pedra::sprite_quadro(int q, bool transparente) {
    return jogo->sprite(PEDRA + (transparente ? TRANSPARENTE : 0));
}

bool Pedra::solido() {
    return true;
}

// Classe Arvore.

#define ARVORE_LARGURA 40
#define ARVORE_ALTURA 80

Arvore::Arvore(int x, int y, Jogo *jogo) :
        Personagem(TIPO_ARVORE, x, y, ARVORE_LARGURA, ARVORE_ALTURA, 2, 100, jogo) {
    this->sprite_offset_x = -6;
    this->sprite_offset_y = -10;
}

Arvore::~Arvore() {
}

BITMAP *Arvore::sprite_quadro(int q, bool transparente) {
    return q ? jogo->sprite(ARVORE_A + (transparente ? TRANSPARENTE : 0)) :
               jogo->sprite(ARVORE_B + (transparente ? TRANSPARENTE : 0));
}

bool Arvore::solido() {
    return true;
}