/*
 * TINS 2010 - The source god
 * Authors: Victor Williams Stafusa da Silva
 *          André Luiz Pereira Álvares
 */
#include "game.h"

class FaseTudoNoInicio : public virtual FaseStart {

  private:
    int comecou;

  protected:
    virtual void faz(Jogo *jogo) = 0;

  public:
    FaseTudoNoInicio() {
        comecou = false;
    }

    bool start(Jogo *jogo) {
        if (comecou) return true;
        faz(jogo);
        comecou = true;
        return true;
    }
};

class FaseIdiota : public virtual FaseTudoNoInicio {

  protected:
    void faz(Jogo *jogo) {
        jogo->add(new HomemPalito(40, 40, jogo));
        jogo->add(new HomemPalito(80, 40, jogo));
        jogo->add(new HomemPalito(40, 80, jogo));
        jogo->add(new HomemPalito(80, 80, jogo));
        jogo->add_nesta_fase(new Bug(40, 40, jogo));
    }
};

class Fase1 : public virtual FaseTudoNoInicio {

  protected:
    void faz(Jogo *jogo) {
        jogo->add(new HomemPalito(100, 300, jogo));
        jogo->add(new HomemPalito(120, 300, jogo));
        jogo->add(new HomemPalito(140, 300, jogo));
        jogo->add_nesta_fase(new Bug(40, 40, jogo));
        jogo->add_nesta_fase(new Bug(720, 40, jogo));
        jogo->add_nesta_fase(new Bug(40, 520, jogo));
        jogo->add_nesta_fase(new Bug(720, 520, jogo));
    }
};

class Fase2 : public virtual FaseTudoNoInicio {

  protected:
    void faz(Jogo *jogo) {
        jogo->add_nesta_fase(new MonstroVerde(320, 100, jogo));
        jogo->add_nesta_fase(new Bug(400, 100, jogo));
        jogo->add_nesta_fase(new MonstroVermelho(520, 100, jogo));

        int i;
        for (i = 40; i <= 720; i += 40) {
            jogo->add_nesta_fase(new Pedra(i, 280, jogo));
        }
        jogo->add_nesta_fase(new Arvore(280, 240, jogo));
    }
};

class Fase3 : public virtual FaseTudoNoInicio {

  protected:
    void faz(Jogo *jogo) {
        jogo->add(new HomemPalito(100, 300, jogo));
        jogo->add_nesta_fase(new MonstroVerde(320, 100, jogo));
        jogo->add_nesta_fase(new Bug(200, 100, jogo));
        jogo->add_nesta_fase(new MonstroVermelho(120, 100, jogo));
        jogo->add_nesta_fase(new Bug(400, 100, jogo));
        jogo->add_nesta_fase(new MonstroVermelho(520, 100, jogo));

        int i;
        bool k = false;
        for (i = 40; i <= 720; i += 40) {
            if (k) jogo->add_nesta_fase(new Pedra(i, 200, jogo));
            else jogo->add_nesta_fase(new Arvore(i, 200, jogo));
            k = !k;
        }
        for (i = 40; i <= 720; i += 40) {
            if (k) jogo->add_nesta_fase(new Pedra(i, 400, jogo));
            else jogo->add_nesta_fase(new Arvore(i, 400, jogo));
            k = !k;
        }
    }
};

class Fase9 : public virtual FaseStart {

  private:
    int tick;

  public:
    bool start(Jogo *jogo) {
        if (tick == 0) jogo->add(new HomemPalito(100, 100, jogo));
        if (tick % 1000 == 0 && tick < 5000) { 
            int i, j;
            for (i = 40; i < 800; i += 200) {
                for (j = 40; j < 600; j += 200) {
                    jogo->add_nesta_fase(new Bug(i, j, jogo));
                }
            }
        }
        if (tick == 3000) jogo->add(new HomemPalito(100, 100, jogo));
        tick++;
        return tick <= 5000;
    }
};

class Fase10 : public virtual FaseStart {

  private:
    int tick;

  public:
    bool start(Jogo *jogo) {
        if (tick == 0) jogo->add(new HomemPalito(100, 100, jogo));
        if (tick % 1000 == 0 && tick < 5000) { 
            int i, j;
            bool k = false;
            for (i = 40; i < 800; i += 200) {
                for (j = 40; j < 600; j += 200) {
                    if (k) jogo->add_nesta_fase(new Bug(i, j, jogo));
                    else jogo->add_nesta_fase(new MonstroVerde(i, j, jogo));
                    k = !k;
                }
            }
        }
        if (tick == 3000) jogo->add(new HomemPalito(100, 100, jogo));
        tick++;
        return tick <= 5000;
    }
};

// Classe ConjuntoFase

ConjuntoFase::ConjuntoFase() {
    fases[0] = new Fase("include/debug.h",        0, "media/mid/0.mid", 0, 0, 0.3f, new Fase1());
    fases[1] = new Fase("src/monstro.cpp",        54, "media/mid/1.mid", 2, 50, 0.3f, new Fase2());
    fases[2] = new Fase("src/character.cpp",      5, "media/mid/2.mid", 2, 50, 0.4f, new Fase3());
    fases[3] = new Fase("include/readsource.h",   5, "media/mid/3.mid", 2, 100, 0.4f, new Fase3());
    fases[4] = new Fase("src/readsource.cpp",     5, "media/mid/4.mid", 2, 100, 0.5f, new Fase3());
    fases[5] = new Fase("include/pathfind.h",     5, "media/mid/5.mid", 2, 100, 0.5f, new Fase3());
    fases[6] = new Fase("src/tela.cpp",           5, "media/mid/6.mid", 2, 100, 0.6f, new Fase3());
    fases[7] = new Fase("src/objetos.cpp",        5, "media/mid/7.mid", 2, 100, 0.6f, new Fase3());
    fases[8] = new Fase("src/homempalito.cpp",    5, "media/mid/8.mid", 2, 100, 0.6f, new Fase9());
    fases[9] = new Fase("src/main.cpp",          20, "media/mid/9.mid", 2, 100, 0.7f, new Fase10());
}

ConjuntoFase::~ConjuntoFase() {
    int i;
    for (i = 0; i < 10; i++) {
        delete fases[i];
    }
}

Fase *ConjuntoFase::get_fase(int numero) {
    return numero < 0 || numero > 9 ? NULL : fases[numero];
}