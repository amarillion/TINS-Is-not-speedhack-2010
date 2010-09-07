/*
 * TINS 2010 - The source god
 * Authors: Victor Williams Stafusa da Silva
 *          André Luiz Pereira Álvares
 */
#include "game.h"

#define TILE_W 40
#define TILE_H 40

// Classe Monstro.

Monstro::Monstro(int tipo, int x, int y, int larg, int alt, int num_quadros, int duracao, Jogo *jogo) :
        Personagem(tipo, x, y, larg, alt, num_quadros, duracao, jogo) {
}

Monstro::~Monstro() {
}

bool Monstro::inimigo() {
    return true;
}

PathFinder *Monstro::destinos() {
    return NULL;
}

void Monstro::chegou() {
}

void Monstro::andando() {
    //debug("La vai");
    //char str[200];

    int fator_larg = TILE_W;
    int fator_alt = TILE_H;
    int htiles = SCREEN_W / fator_larg;
    //int vtiles = SCREEN_H / fator_alt;

    int cx = this->x + (this->larg / 2);
    int cy = this->y + (this->alt / 2);
    int meu_x = cx / fator_larg;
    int meu_y = cy / fator_alt;
    int meu_vertice = meu_y * htiles + meu_x;

    int quero_vertice = (quero_x == -1 || quero_y == -1) ? -1 : quero_y * htiles + quero_x;
    int cqx = quero_x * fator_larg;
    int cqy = quero_y * fator_alt;

    /*sprintf(str, "Meu vertice eh %d, %d, %d e %d e meu destino eh %d",
            meu_vertice1, meu_vertice2, meu_vertice3, meu_vertice4, quero_vertice);
    debug(str);*/

    if (quero_vertice != -1 && meu_vertice != quero_vertice && (cqx != x || cqy != y)) {
        int mx = limitar(cqx - x, -2, 2);
        int my = limitar(cqy - y, -2, 2);
        mover(mx, my);
        return;
    }

    PathFinder *p = destinos();
    Area *a = (p == NULL ? NULL : new Area(meu_vertice, p->get_grid()));
    if (p != NULL) delete p;

    if (a == NULL || !a->achou_caminho()) {
        //debug("Sem destino");
        return;
    }

    std::list<int> caminho = a->caminho();
    std::list<int>::iterator it = caminho.begin();
    while (it != caminho.end() && *it == meu_vertice) {
        it++;
    }
    if (it == caminho.end()) {
        delete a;
        chegou();
        return;
    }
    int vertice_destino = *it;
    delete a;

    //sprintf(str, "Meu novo destino eh %d (x=%d, y=%d)", vertice_destino, vertice_destino % htiles, vertice_destino / htiles);
    //debug(str);

    quero_x = vertice_destino % htiles;
    quero_y = vertice_destino / htiles;
    int delta_x = limitar((quero_x * fator_larg) - x, -2, 2);
    int delta_y = limitar((quero_y * fator_alt) - y, -2, 2);

    if (!mover(delta_x, delta_y)) {
        quero_x = meu_x;
        quero_y = meu_y;
    }
}