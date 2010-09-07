/*
 * TINS 2010 - The source god
 * Authors: Victor Williams Stafusa da Silva
 *          André Luiz Pereira Álvares
 */
#include "game.h"

#define COM_PESOS 0

// Classe ViolenciaContraPalitos

ViolenciaContraPalitos::ViolenciaContraPalitos() {
}

void ViolenciaContraPalitos::processa(Personagem *p, VizinhancaGrid *v) {
    int htiles = v->largura();
    int vtiles = v->altura();
    int fator_larg = SCREEN_W / htiles;
    int fator_alt = SCREEN_H / vtiles;

    int x1 = (p->get_x() + p->get_largura() / 2) / fator_larg;
    int y1 = (p->get_y() + p->get_altura() / 2) / fator_alt;

    int idx = x1 + htiles * y1;
    if (p->solido()) {
        v->set_obstaculo(idx);
    } else if (p->amigo() && p->get_hp() > 0) {
#if COM_PESOS
        float peso = (v->is_destino(idx) ? v->get_peso(idx) : 0.0);
        v->set_destino(idx, peso + 1);
#else
        v->set_destino(idx, 1);
#endif
    }
}

// Class PerseguePalitoPathFinder

PerseguePalitoPathFinder::PerseguePalitoPathFinder(int fator_larg, int fator_alt, PathFindFiltro *f, Jogo *jogo) {
    int htiles = SCREEN_W / fator_larg;
    int vtiles = SCREEN_H / fator_alt;
    this->g = new VizinhancaGrid(htiles, vtiles);
    this->f = f;

    std::list<Personagem *> *pers = jogo->get_personagens();
    std::list<Personagem *>::iterator itp;
    for (itp = pers->begin(); itp != pers->end(); itp++) {
        Personagem *p = *itp;
        f->processa(p, g);
    }
}

PerseguePalitoPathFinder::~PerseguePalitoPathFinder() {
    delete g;
    delete f;
}

VizinhancaGrid *PerseguePalitoPathFinder::get_grid() {
    return g;
}

// Classe ComeLetraPerseguePalitoPathFinder

ComeLetraPerseguePalitoPathFinder::ComeLetraPerseguePalitoPathFinder(int fator_larg, int fator_alt, PathFindFiltro *f, Jogo *jogo) :
        PerseguePalitoPathFinder(fator_larg, fator_alt, f, jogo) {

    int htiles = SCREEN_W / fator_larg;
    CharacterIterator *cit = jogo->iterate_source();
    while (cit != NULL && cit->has_next()) {
        Character *c = cit->next();
        if (!c->is_valid() || !c->is_visible()) continue;
        int x1 = (c->get_x() * 8 + 35) / fator_larg;
        int y1 = (c->get_y() * 20 + 30) / fator_alt;
        int idx = x1 + htiles * y1;

#if COM_PESOS
        float peso = (g->is_destino(idx) ? g->get_peso(idx) : 0.0);
        g->set_destino(idx, peso + 1);
#else
        g->set_destino(idx, 1);
#endif
    }
}