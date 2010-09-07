/*
 * TINS 2010 - The source god
 * Authors: Victor Williams Stafusa da Silva
 *          André Luiz Pereira Álvares
 */
#include "game.h"

EstadoMao::EstadoMao(Mao *mao) {
    this->mao = mao;
}

void EstadoMao::agir() {
}

#define READ_MOUSE(x, y) \
    int pos = mouse_pos; \
    int x = pos >> 16; \
    int y = pos & 0x0000ffff; \

class Divina : public EstadoMao {

  public:
    Divina(Mao *mao) : EstadoMao(mao) {}

    void mudar() {
        mao->estado_teclado();
    }

    void agir() {
        if (!(mouse_b & 1)) return;
        READ_MOUSE(mouse_x, mouse_y)

        std::list<Personagem*> *personagens = mao->jogo->get_personagens();
        std::list<Personagem*>::iterator it_personagens;
        for (it_personagens = personagens->begin(); it_personagens != personagens->end(); it_personagens++) {
            Personagem *p = *it_personagens;
            if (p->aceita_clique(mouse_x, mouse_y, true)) {
                if (mao->possuido == p) return;
                if (mao->possuido != NULL) mao->possuido->unselect();
                mao->possuido = p;
                mao->estado = mao->possuir;
                return;
            }
        }
    }

    int valor() {
        return -1;
    }

    BITMAP *sprite() {
        return mao->jogo->sprite(DIVINA);
    }
};

class MaoAberta : public EstadoMao {

  private:
    void fechar_mao() {
        READ_MOUSE(mouse_x, mouse_y)

        std::list<Personagem*> *personagens = mao->jogo->get_personagens();
        std::list<Personagem*>::iterator it_personagens;
        for (it_personagens = personagens->begin(); it_personagens != personagens->end(); it_personagens++) {
            Personagem *p = *it_personagens;
            if (p->aceita_clique(mouse_x, mouse_y, false)) {
                mao->na_mao = p;
                p->set_na_mao(true);
                mao->estado = mao->mao_fechada;
                return;
            }
        }
    }

  public:
    MaoAberta(Mao *mao) : EstadoMao(mao) {}

    void mudar() {
        if ((mouse_b & 1) && mao->qtd_maos > 0) {
            mao->qtd_maos--;
            fechar_mao();
        } else {
            mao->estado_teclado();
        }
    }

    int valor() {
        return mao->qtd_maos;
    }

    BITMAP *sprite() {
        return mao->jogo->sprite(MAO1);
    }
};

class MaoFechada : public EstadoMao {

  public:
    MaoFechada(Mao *mao) : EstadoMao(mao) {}

    void mudar() {
        if (mouse_b & 1) return;
        READ_MOUSE(mouse_x, mouse_y)
        mao->na_mao->posicionar(mouse_x, mouse_y);
        mao->na_mao->set_na_mao(false);
        mao->na_mao = NULL;
        mao->estado = mao->mao_aberta;
    }

    int valor() {
        return mao->qtd_maos;
    }

    BITMAP *sprite() {
        return mao->jogo->sprite(MAO2);
    }
};

class Possuida : public EstadoMao {

  public:
    Possuida(Mao *mao) : EstadoMao(mao) {}

    void mudar() {
        if (key[KEY_ENTER]) mao->unselect();
    }

    int valor() {
        return -1;
    }

    BITMAP *sprite() {
        return NULL;
    }
};

class Spray : public EstadoMao {

  public:
    Spray(Mao *mao) : EstadoMao(mao) {}

    void mudar() {
        if ((mouse_b & 1) && mao->qtd_spray > 0) {
            mao->estado = mao->aplicando_spray;
        } else {
            mao->estado_teclado();
        }
    }

    int valor() {
        return mao->qtd_spray;
    }

    BITMAP *sprite() {
        return mao->jogo->sprite(SPRAY1);
    }
};

class AplicandoSpray : public EstadoMao {

  public:
    AplicandoSpray(Mao *mao) : EstadoMao(mao) {}

    void agir() {
        READ_MOUSE(mouse_x, mouse_y)

        FiltroMataMonstro f;
        std::list<Personagem *> achou;
        mao->jogo->personagens_na_area(mouse_x - 5, mouse_y - 5, 10, 10, &achou, &f);

        std::list<Personagem *>::iterator it;
        for (it = achou.begin(); it != achou.end(); it++) {
            Personagem *p = *it;
            p->apanhar(1);
        }
        mao->qtd_spray--;
    }

    int valor() {
        return mao->qtd_spray;
    }

    void mudar() {
        if (!(mouse_b & 1) || mao->qtd_spray == 0) mao->estado = mao->spray;
    }

    BITMAP *sprite() {
        return mao->jogo->sprite(SPRAY2);
    }
};

Mao::Mao(Jogo *jogo) {
    this->jogo = jogo;
    this->possuido = NULL;
    this->na_mao = NULL;

    this->divina = new Divina(this);
    this->possuir = new Possuida(this);
    this->mao_aberta = new MaoAberta(this);
    this->mao_fechada = new MaoFechada(this);
    this->spray = new Spray(this);
    this->aplicando_spray = new AplicandoSpray(this);

    this->estado = divina;
}

Mao::~Mao() {
    delete divina;
    delete possuir;
    delete mao_aberta;
    delete mao_fechada;
    delete spray;
    delete aplicando_spray;
}

void Mao::render(BITMAP *bmp) {
    BITMAP *mao = estado->sprite();
    if (mao == NULL) return;

    READ_MOUSE(mouse_x, mouse_y)
    draw_sprite(bmp, mao, mouse_x - 25, mouse_y + 1);
    int v = estado->valor();
    if (v != -1) {
        textprintf_centre_ex(bmp, font, mouse_x - 21 + (mao->w / 2), mouse_y + (mao->h / 2) + 5, BRANCO, -1, "%d", v);
    }
}

void Mao::unselect() {
    this->estado = divina;
    if (possuido != NULL) possuido->unselect();
    possuido = NULL;
}

void Mao::agir() {
    estado->mudar();
    estado->agir();
}

void Mao::estado_teclado() {
    if (key[KEY_1]) {
        estado = divina;
    } else if (key[KEY_2]) {
        estado = mao_aberta;
    } else if (key[KEY_3]) {
        estado = spray;
    }
}

void Mao::set_sprays(int qtd) {
    this->qtd_spray = qtd;
}

void Mao::set_maos(int qtd) {
    this->qtd_maos = qtd;
}