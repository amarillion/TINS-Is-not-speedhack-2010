/*
 * TINS 2010 - The source god
 * Authors: Victor Williams Stafusa da Silva
 *          André Luiz Pereira Álvares
 */
#ifndef __SENTINELA_GAME
#define __SENTINELA_GAME

#include <allegro.h>
#include <list>
#include <map>
#include <vector>

#include <iostream>
#include <fstream>

#include "debug.h"
#include "pathfind.h"
#include "readsource.h"

/* Definições básicas. */

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

/* Definição de cores. */

#define PRETO            makecol(0, 0, 0)
#define VERDE            makecol(0, 255, 0)
#define VERMELHO         makecol(255, 0, 0)
#define COR_MAO          makecol(128, 0, 0)
#define BRANCO           makecol(255, 255, 255)
#define AMARELO          makecol(255, 255, 0)
#define MAGENTA          makecol(255, 0, 255)
#define AZUL             makecol(0, 0, 255)

/* Definição de sprites. */

#define TRANSPARENTE 10000
#define ROTATE_LEFT   2000
#define ROTATE_RIGHT  4000
#define ROTATE_180    6000

#define HOMEM_PALITO_A 1
#define HOMEM_PALITO_B 2
#define HOMEM_PALITO_ATACA 3
#define HOMEM_PALITO_APANHA 4
#define HOMEM_PALITO_AREA 5

#define MONSTRO_VERDE_A 101
#define MONSTRO_VERDE_B 102
#define MONSTRO_VERDE_ATACA 103
#define MONSTRO_VERDE_APANHA 104

#define MONSTRO_VERMELHO_A 201
#define MONSTRO_VERMELHO_B 202
#define MONSTRO_VERMELHO_ATACA 203
#define MONSTRO_VERMELHO_APANHA 204

#define BUG_A 301
#define BUG_B 302
#define BUG_ATACA 303
#define BUG_APANHA 304

#define BARRA_VAZIA 1002
#define BARRA_VERDE 1003
#define BARRA_AMARELA 1004
#define BARRA_VERMELHA 1005
#define MAO1 1006
#define MAO2 1007
#define DIVINA 1008
#define SPRAY1 1009
#define SPRAY2 1010

#define PEDRA 1101

#define ARVORE_A 1201
#define ARVORE_B 1202

#define GAME_OVER 1301
#define CONGRATS 1302

#define SOUND_ATTACK 1701
#define SOUND_DIE 1702
#define SOUND_AHHH 1703

/* Tag de tipos de personagens. */

#define TIPO_HOMEM_PALITO 1

#define TIPO_MONSTRO_VERDE 11
#define TIPO_MONSTRO_VERMELHO 12
#define TIPO_BUG 13

#define TIPO_PEDRA 101
#define TIPO_ARVORE 102

/* Estado dos personagens. */

#define ANDANDO 1
#define ATACANDO 2
#define APANHANDO 3
#define MORTO 4

/* Definição de funções. */

int sinal(int x);
int limitar(int valor, int min, int max);
void desenha_linhas(BITMAP *tela);
bool colide(int a1, int a2, int b1, int b2);

/* Definição de classes. */

class Jogo;
class Filtro;
class Mao;

class Personagem {
  private:
    int tipo;
    bool anda_pixel(int delta_x, int delta_y);
    virtual void apanhando();
    virtual void atacando();
    virtual void morrendo();

  protected:
    Jogo *jogo;
    int quadro;
    int num_quadros;
    int duracao;
    int x, y, larg, alt, hp, hp_max, sprite_offset_x, sprite_offset_y;
    int quero_x, quero_y;
    int direcao_x, direcao_y;
    int estado;
    int tempo_ataque;
    int tempo_apanha;
    int tempo_morre;
    int forca;
    int parada;
    bool na_mao;
    bool selecionado;

    Personagem(int tipo, int x, int y, int larg, int alt, int num_quadros, int duracao, Jogo *jogo);
    virtual BITMAP *sprite_quadro(int q, bool transparente) = 0;
    virtual void andando();
    bool atacar(Filtro *f);
    virtual void morreu();

  public:
    virtual ~Personagem();
    int get_x();
    int get_y();
    int get_largura();
    int get_altura();
    int get_tipo();
    int get_hp();
    virtual bool inimigo();
    virtual bool amigo();
    virtual bool solido();

    void render(BITMAP *buffer, bool transparente);
    void proximo_quadro();
    virtual bool aceita_clique(int x, int y, bool selecionar);

    bool mover(int delta_x, int delta_y);
    void posicionar(int x, int y);
    void agir();
    void apanhar(int qtd);
    void set_na_mao(bool na_mao);
    bool is_na_mao();
    void unselect();
};

class HomemPalito : public Personagem {
  private:
    void consertar_texto();

  protected:
    BITMAP *sprite_quadro(int q, bool transparente);
    void andando();
    void morreu();

  public:
    HomemPalito(int x, int y, Jogo *jogo);
    virtual ~HomemPalito();
    bool aceita_clique(int x, int y, bool selecionar);
    bool amigo();
};

class Monstro : public Personagem {
  protected:
    Monstro(int tipo, int x, int y, int larg, int alt, int num_quadros, int duracao, Jogo *jogo);
    void andando();

  public:
    virtual ~Monstro();
    bool inimigo();
    virtual PathFinder *destinos();
    virtual void chegou();
};

class MonstroVerde : public Monstro {
  protected:
    BITMAP *sprite_quadro(int q, bool transparente);

  public:
    MonstroVerde(int x, int y, Jogo *jogo);
    virtual ~MonstroVerde();
    PathFinder *destinos();
    void chegou();
    void morreu();
};

class MonstroVermelho : public Monstro {
  protected:
    BITMAP *sprite_quadro(int q, bool transparente);

  public:
    MonstroVermelho(int x, int y, Jogo *jogo);
    virtual ~MonstroVermelho();
    PathFinder *destinos();
    void chegou();
};

class Bug : public Monstro {
  protected:
    BITMAP *sprite_quadro(int q, bool transparente);

  public:
    Bug(int x, int y, Jogo *jogo);
    virtual ~Bug();
    PathFinder *destinos();
    void chegou();
    void atacar_texto();
};

class Pedra : public Personagem {
  protected:
    BITMAP *sprite_quadro(int q, bool transparente);

  public:
    Pedra(int x, int y, Jogo *jogo);
    virtual ~Pedra();
    bool solido();
};

class Arvore : public Personagem {
  protected:
    BITMAP *sprite_quadro(int q, bool transparente);

  public:
    Arvore(int x, int y, Jogo *jogo);
    virtual ~Arvore();
    bool solido();
};

class FaseStart {
  public:
    virtual bool start(Jogo *jogo) = 0;
};

class Fase {
  private:
    int linhainicial;
    MIDI *music;
    std::list<std::list<Character*>*>* srccode;
    int char_count;
    int total_char;
    FaseStart *logic;
    Jogo *jogo;
    int sprays;
    int maos;
    float codigo_ok;

  public:
    Fase(const char *file, const int linhainicial, const char *midi, int maos, int sprays, float codigo_ok, FaseStart *logic);
    virtual ~Fase();
    void start(Jogo *jogo);
    bool tick();
    void inc_char_count();
    void dec_char_count();
    CharacterIterator *iterate_source();
    int get_sprays();
    int get_maos();
    bool is_codigo_ok();
    float get_codigo_qtd();
    float get_codigo_ok();
};

class ConjuntoFase {
  private:
    std::map<int, Fase *> fases;

  public:
    ConjuntoFase();
    virtual ~ConjuntoFase();
    Fase *get_fase(int numero);
};

class Tela {
  private:
    BITMAP *tela;

  public:
    Tela();
    ~Tela();
    BITMAP *getTela();
    void render();
};

class EstadoMao {
  protected:
    Mao *mao;

  public:
    EstadoMao(Mao *mao);
    virtual void agir();
    virtual void mudar() = 0;
    virtual BITMAP *sprite() = 0;
    virtual int valor() = 0;
};

class Mao {
  public:
    Jogo *jogo;
    Personagem *possuido;
    Personagem *na_mao;

    EstadoMao *divina;
    EstadoMao *possuir;
    EstadoMao *mao_aberta;
    EstadoMao *mao_fechada;
    EstadoMao *spray;
    EstadoMao *aplicando_spray;

    EstadoMao *estado;
    int qtd_spray;
    int qtd_maos;
    
    Mao(Jogo *jogo);
    ~Mao();
    void render(BITMAP *bmp);
    void agir();
    void unselect();
    void estado_teclado();
    void set_sprays(int qtd);
    void set_maos(int qtd);
};

class Ambiente {
  public:
    virtual void render() = 0;
    virtual void do_logic() = 0;
};

class Filtro {
  public:
    virtual bool aceita(Personagem *p) = 0;
};

class FiltroMataMonstro : public virtual Filtro {
  public:
    FiltroMataMonstro();
    bool aceita(Personagem* p);
};

class PathFindFiltro {
  public:
    virtual void processa(Personagem *p, VizinhancaGrid *v);
};

class Jogo : public virtual Ambiente {
  private:
    Tela *tela;
    BITMAP *debug_bmp;
    Mao *mao;
    bool game_end;

    std::map<int, BITMAP *> sprites;
    std::map<int, SAMPLE *> sons;
    std::list<BITMAP*> bitmaps;
    std::list<Personagem*> personagens;
    std::list<Personagem*> mortos;
    std::list<Personagem*> matar_no_fim_da_fase;
    ConjuntoFase fases;

    int inimigos_vivos;
    int amigos_vivos;
    int fase_atual;

    void select();
    void garbage_collect();
    void full_garbage_collect();
    void nova_fase();

  public:
    Jogo();
    ~Jogo();
    void render();
    void do_logic();
    BITMAP *sprite(int numero);
    void play_som(int numero, int freq);
    void remove(Personagem *);
    void add(Personagem *);
    void add_nesta_fase(Personagem *p);
    void personagens_na_area(int x, int y, int largura, int altura, std::list<Personagem *>* lista, Filtro *f);
    void unselect();
    void inc_char_count();
    void dec_char_count();
    bool game_over();
    bool game_win();
    std::list<Personagem *> *get_personagens();
    CharacterIterator *iterate_source();
};

class MenuInicial : public virtual Ambiente {
  private:
    Tela *tela;
    Jogo *j;
    int fontcolor;
    MIDI *music;
    std::list<std::list<Character*>*>* srccode;

  public:
    MenuInicial();
    ~MenuInicial();
    void render();
    void do_logic();
};

class PerseguePalitoPathFinder : public virtual PathFinder {
  protected:
    VizinhancaGrid *g;
    PathFindFiltro *f;

  public:
    PerseguePalitoPathFinder(int fator_larg, int fator_alt, PathFindFiltro *f, Jogo *jogo);
    ~PerseguePalitoPathFinder();
    VizinhancaGrid *get_grid();
};

class ComeLetraPerseguePalitoPathFinder : public PerseguePalitoPathFinder {
  public:
    ComeLetraPerseguePalitoPathFinder(int fator_larg, int fator_alt, PathFindFiltro *f, Jogo *jogo);
};

class ViolenciaContraPalitos : public virtual PathFindFiltro {
  public:
    ViolenciaContraPalitos();
    void processa(Personagem *p, VizinhancaGrid *v);
};

#endif /* __SENTINELA_GAME */