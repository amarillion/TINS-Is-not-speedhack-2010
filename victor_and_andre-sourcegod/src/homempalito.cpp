/*
 * TINS 2010 - The source god
 * Authors: Victor Williams Stafusa da Silva
 *          André Luiz Pereira Álvares
 */
#include "game.h"

#define HOMEM_PALITO_LARGURA 18
#define HOMEM_PALITO_ALTURA 42

// Classe FiltroMataMonstro

FiltroMataMonstro::FiltroMataMonstro() {
}

bool FiltroMataMonstro::aceita(Personagem* p) {
    return p->inimigo() && p->get_hp() > 0;
}

// Classe HomemPalito

HomemPalito::HomemPalito(int x, int y, Jogo *jogo) :
        Personagem(TIPO_HOMEM_PALITO, x, y, HOMEM_PALITO_LARGURA, HOMEM_PALITO_ALTURA, 2, 40, jogo) {
    this->sprite_offset_x = -2;
    this->sprite_offset_y = -1;
    this->hp_max = this->hp = 50;
    this->forca = 5;
    this->tempo_ataque = 3;
    this->tempo_apanha = 3;
    this->tempo_morre = 10;
}

HomemPalito::~HomemPalito() {
    if (selecionado) jogo->unselect();
}

bool HomemPalito::aceita_clique(int x, int y, bool selecionar) {
    if (jogo->game_over() || jogo->game_win()) return false;
    BITMAP *area = jogo->sprite(HOMEM_PALITO_AREA);
    if (x > this->x + area ->w || x < this->x || y > this->y + area ->h || y < this->y) return false;
    int pixel = getpixel(area, x - this->x, y - this->y);
    bool ret = (pixel == PRETO);
    if (selecionar && ret && !selecionado) {
        selecionado = true;
    }
    return !selecionar || selecionado;
}

BITMAP *HomemPalito::sprite_quadro(int q, bool transparente) {
    int flag = (estado == ATACANDO ? HOMEM_PALITO_ATACA :
                estado == APANHANDO || estado == MORTO ? HOMEM_PALITO_APANHA :
                q ? HOMEM_PALITO_A : HOMEM_PALITO_B);

    return jogo->sprite(flag + (transparente ? TRANSPARENTE : 0));
}

bool HomemPalito::amigo() {
    return true;
}

void HomemPalito::andando() {
    if (!selecionado) return;
    if (jogo->game_over() || jogo->game_win()) {
        jogo->unselect();
        selecionado = false;
        return;
    }
    if (key[KEY_SPACE]) {
        jogo->play_som(SOUND_ATTACK, 1000);
        FiltroMataMonstro f;
        atacar(&f);
        consertar_texto();
        return;
    }

    int delta_x = 0;
    int delta_y = 0;
    if (key[KEY_1_PAD] || key[KEY_2_PAD] || key[KEY_3_PAD] || key[KEY_DOWN]) { delta_y++; }
    if (key[KEY_1_PAD] || key[KEY_4_PAD] || key[KEY_7_PAD] || key[KEY_LEFT]) { delta_x--; }
    if (key[KEY_3_PAD] || key[KEY_6_PAD] || key[KEY_9_PAD] || key[KEY_RIGHT]) { delta_x++; }
    if (key[KEY_7_PAD] || key[KEY_8_PAD] || key[KEY_9_PAD] || key[KEY_UP]) { delta_y--; }

    delta_x = sinal(delta_x) * 2;
    delta_y = sinal(delta_y) * 2;
    this->mover(delta_x, delta_y);
}

void HomemPalito::morreu() {
    if (selecionado) jogo->unselect();
    selecionado = false;
    jogo->play_som(rand() % 2 == 0 ? SOUND_DIE : SOUND_AHHH, 1000);
}

void HomemPalito::consertar_texto() {
    CharacterIterator *cit = jogo->iterate_source();

    while (cit != NULL && cit->has_next()) {
        Character *c = cit->next();
        if (!c->is_valid() || c->is_visible()) continue;
        int x1 = c->get_x() * 8 + 35;
        int y1 = c->get_y() * 20 + 30;
        bool colide_x = colide(x, x + larg, x1, x1 + 8);
        bool colide_y = colide(y, y + alt, y1, y1 + 8);

        if (colide_x && colide_y) {
            c->set_visible(true);
            jogo->inc_char_count();
            break;
        }
    }

    delete cit;
}