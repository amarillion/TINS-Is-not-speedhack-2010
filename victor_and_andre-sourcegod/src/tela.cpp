/*
 * TINS 2010 - The source god
 * Authors: Victor Williams Stafusa da Silva
 *          André Luiz Pereira Álvares
 */
#include "game.h"

#define LINE_COLOR makecol(128, 128, 255)
#define MARGIN_COLOR makecol(255, 128, 128)

void desenha_linhas(BITMAP *tela) {
    // desenha as margens
    vline(tela, 30, 0, SCREEN_HEIGHT, MARGIN_COLOR);
    vline(tela, SCREEN_WIDTH - 30, 0, SCREEN_HEIGHT, MARGIN_COLOR);

    // desenhas as linhas
    for (int i = 20; i < SCREEN_HEIGHT; i += 20) {
        hline(tela, 0, i + 20, SCREEN_WIDTH, LINE_COLOR);
    }
}

Tela::Tela() {
    this->tela = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
}

Tela::~Tela() {
    destroy_bitmap(tela);
}

BITMAP *Tela::getTela() {
    return tela;
}

void Tela::render() {
    clear_to_color(tela, BRANCO);
    desenha_linhas(tela);
}