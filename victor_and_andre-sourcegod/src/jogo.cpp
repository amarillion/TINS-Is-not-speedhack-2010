/*
 * TINS 2010 - The source god
 * Authors: Victor Williams Stafusa da Silva
 *          André Luiz Pereira Álvares
 */
#include "game.h"

class SpriteError {
  private:
    const char *mensagem;

  public:
    SpriteError(const char *mensagem) {
        this->mensagem = mensagem;
    }

    const char *get_mensagem() {
        return mensagem;
    }
};

static void replace_color(BITMAP *b, int old_color, int replacement_color) {
    int i, j, p;
    for (j = 0; j < b->h; j++) {
        for (i = 0; i < b->w; i++) {
            p = getpixel(b, i, j);
            putpixel(b, i, j, p == old_color ? replacement_color : p);
        }
    }
}

static BITMAP *replace_color_bitmap(BITMAP *b, int old_color, int replacement_color) {
    BITMAP *novo = create_bitmap(b->w, b->h);

    int i, j, p;
    for (j = 0; j < b->h; j++) {
        for (i = 0; i < b->w; i++) {
            p = getpixel(b, i, j);
            putpixel(novo, i, j, p == old_color ? replacement_color : p);
        }
    }

    return novo;
}

static BITMAP *create_rotate_90(BITMAP *b) {
    BITMAP *novo = create_bitmap(b->w, b->h);

    int i, j, p;
    for (j = 0; j < b->h; j++) {
        for (i = 0; i < b->w; i++) {
            p = getpixel(b, j, i);
            putpixel(novo, i, j, p);
        }
    }

    return novo;
}

static BITMAP *create_rotate_180(BITMAP *b) {
    BITMAP *novo = create_bitmap(b->w, b->h);

    int i, j, p;
    for (j = 0; j < b->h; j++) {
        for (i = 0; i < b->w; i++) {
            p = getpixel(b, b->h - i - 1, b->w - j - 1);
            putpixel(novo, i, j, p);
        }
    }

    return novo;
}

static BITMAP *create_rotate_270(BITMAP *b) {
    BITMAP *novo = create_bitmap(b->w, b->h);

    int i, j, p;
    for (j = 0; j < b->h; j++) {
        for (i = 0; i < b->w; i++) {
            p = getpixel(b, j, b->h - i - 1);
            putpixel(novo, i, j, p);
        }
    }

    return novo;
}

#define CARREGAR_SOM(chave) \
    debug("Abrindo media/wav/" #chave ".wav"); \
    s = load_sample("media/wav/" #chave ".wav"); \
    if (s == NULL) { \
        debug("FALHOU!"); \
        throw new SpriteError(#chave); \
    } \
    sons[chave] = s;

#define CARREGAR_SPRITE(chave, rot) \
    debug("Abrindo media/img/" #chave ".bmp"); \
    o = load_bitmap("media/img/" #chave ".bmp", NULL); \
    if (o == NULL) { \
        debug("FALHOU!"); \
        throw new SpriteError(#chave); \
    } \
    sprites[chave] = o; \
    t = replace_color_bitmap(o, BRANCO, MAGENTA); \
    sprites[TRANSPARENTE + chave] = t; \
    if (rot) { \
        r = create_rotate_90(o); \
        sprites[chave + ROTATE_LEFT] = r; \
        r = replace_color_bitmap(r, BRANCO, MAGENTA); \
        sprites[chave + ROTATE_LEFT + TRANSPARENTE] = r; \
        r = create_rotate_270(o); \
        sprites[chave + ROTATE_RIGHT] = r; \
        r = replace_color_bitmap(r, BRANCO, MAGENTA); \
        sprites[chave + ROTATE_RIGHT + TRANSPARENTE] = r; \
        r = create_rotate_180(o); \
        sprites[chave + ROTATE_180] = r; \
        r = replace_color_bitmap(r, BRANCO, MAGENTA); \
        sprites[chave + ROTATE_180 + TRANSPARENTE] = r; \
    }

Jogo::Jogo() {
    this->inimigos_vivos = 0;
    this->amigos_vivos = 0;
    this->game_end = false;

    BITMAP *t;
    BITMAP *o;
    BITMAP *r;
    SAMPLE *s;

    debug_bmp = NULL; //create_bitmap(SCREEN_W, SCREEN_H);
    //clear_to_color(debug_bmp, MAGENTA);

    CARREGAR_SPRITE(BARRA_VAZIA, false);
    CARREGAR_SPRITE(BARRA_VERMELHA, false);
    CARREGAR_SPRITE(MAO1, false);
    CARREGAR_SPRITE(MAO2, false);

    CARREGAR_SPRITE(HOMEM_PALITO_A, false);
    CARREGAR_SPRITE(HOMEM_PALITO_B, false);
    CARREGAR_SPRITE(HOMEM_PALITO_ATACA, false);
    CARREGAR_SPRITE(HOMEM_PALITO_APANHA, false);
    CARREGAR_SPRITE(HOMEM_PALITO_AREA, false);

    CARREGAR_SPRITE(MONSTRO_VERDE_A, false);
    CARREGAR_SPRITE(MONSTRO_VERDE_B, false);
    CARREGAR_SPRITE(MONSTRO_VERDE_ATACA, false);
    CARREGAR_SPRITE(MONSTRO_VERDE_APANHA, false);

    CARREGAR_SPRITE(MONSTRO_VERMELHO_A, false);
    CARREGAR_SPRITE(MONSTRO_VERMELHO_B, false);
    CARREGAR_SPRITE(MONSTRO_VERMELHO_ATACA, false);
    CARREGAR_SPRITE(MONSTRO_VERMELHO_APANHA, false);

    CARREGAR_SPRITE(BUG_A, true);
    CARREGAR_SPRITE(BUG_B, true);
    CARREGAR_SPRITE(BUG_ATACA, true);
    CARREGAR_SPRITE(BUG_APANHA, true);

    CARREGAR_SPRITE(PEDRA, false);
    CARREGAR_SPRITE(ARVORE_A, false);
    CARREGAR_SPRITE(ARVORE_B, false);

    CARREGAR_SPRITE(GAME_OVER, false);
    CARREGAR_SPRITE(CONGRATS, false);

    CARREGAR_SOM(SOUND_ATTACK);
    CARREGAR_SOM(SOUND_DIE);
    CARREGAR_SOM(SOUND_AHHH);

    t = replace_color_bitmap(sprites[BARRA_VERMELHA], VERDE, makecol(0, 128, 0));
    replace_color(t, AZUL, makecol(0, 192, 0));
    replace_color(t, AMARELO, makecol(128, 255, 128));
    sprites[BARRA_VERDE] = t;

    t = replace_color_bitmap(sprites[BARRA_VERMELHA], VERDE, makecol(128, 128, 0));
    replace_color(t, AZUL, makecol(192, 192, 0));
    replace_color(t, AMARELO, makecol(255, 255, 0));
    sprites[BARRA_AMARELA] = t;

    t = sprites[BARRA_VERMELHA];
    replace_color(t, VERDE, makecol(224, 0, 0));
    replace_color(t, AZUL, makecol(255, 64, 64));
    replace_color(t, AMARELO, makecol(255, 160, 160));
    sprites[BARRA_VERMELHA] = t;

    sprites[DIVINA] = replace_color_bitmap(sprites[MAO1], COR_MAO, makecol(0, 255, 255));
    sprites[SPRAY1] = replace_color_bitmap(sprites[MAO1], COR_MAO, VERDE);
    sprites[SPRAY2] = replace_color_bitmap(sprites[MAO2], COR_MAO, VERDE);

    this->mao = new Mao(this);
    this->tela = new Tela();

    show_mouse(NULL);

    fase_atual = -1;
    nova_fase();
}

Jogo::~Jogo() {
    garbage_collect();

    std::list<Personagem*>::iterator it_personagens;
    for (it_personagens = personagens.begin(); it_personagens != personagens.end(); it_personagens++) {
        delete *it_personagens;
    }

    std::map<int, BITMAP*>::iterator it_bitmaps;
    for (it_bitmaps = sprites.begin(); it_bitmaps != sprites.end(); it_bitmaps++) {
        free((*it_bitmaps).second);
    }

    std::map<int, SAMPLE*>::iterator it_sons;
    for (it_sons = sons.begin(); it_sons != sons.end(); it_sons++) {
        destroy_sample((*it_sons).second);
    }

    delete mao;
}

void Jogo::garbage_collect() {
    std::list<Personagem*>::iterator it_mortos;
    for (it_mortos = mortos.begin(); it_mortos != mortos.end(); it_mortos++) {
        delete *it_mortos;
    }
    mortos.clear();
}

void Jogo::full_garbage_collect() {
    std::list<Personagem*>::iterator it_mortos;
    for (it_mortos = matar_no_fim_da_fase.begin(); it_mortos != matar_no_fim_da_fase.end(); it_mortos++) {
        Personagem *p = *it_mortos;
        personagens.remove(p);
        mortos.push_front(p);
    }
    matar_no_fim_da_fase.clear();
    garbage_collect();
}

void Jogo::nova_fase() {
    fase_atual++;
    Fase *proxima_fase = fases.get_fase(fase_atual);
    if (proxima_fase == NULL) game_end = true;

    full_garbage_collect();

    char str[50];
    sprintf(str, "Iniciando fase %d.", fase_atual + 1);
    debug(str);

    mao->set_sprays(proxima_fase->get_sprays());
    mao->set_maos(proxima_fase->get_maos());
    proxima_fase->start(this);

    sprintf(str, "Fase %d iniciada.", fase_atual + 1);
    debug(str);
}

void Jogo::do_logic() {

    garbage_collect();
    Fase *atual = fases.get_fase(fase_atual);

    bool next = (atual == NULL ? true : atual->tick());
    if (next && inimigos_vivos == 0 && !game_over() && !game_win()) {
        if (fase_atual == 9) {
            game_end = true;
        } else {
            nova_fase();
        }
    }

    if (!game_over() && !game_win()) mao->agir();

    std::list<Personagem*>::iterator it_personagens;
    for (it_personagens = personagens.begin(); it_personagens != personagens.end(); it_personagens++) {
        Personagem *p = *it_personagens;
        if (p->is_na_mao()) continue;
        p->agir();
    }
}

CharacterIterator *Jogo::iterate_source() {
    Fase *fase = fases.get_fase(fase_atual);
    return fase == NULL ? NULL : fase->iterate_source();
}

void Jogo::inc_char_count() {
    Fase *fase = fases.get_fase(fase_atual);
    if (fase == NULL) return; // Não deveria ocorrer nunca.
    fase->inc_char_count();
}

void Jogo::dec_char_count() {
    Fase *fase = fases.get_fase(fase_atual);
    if (fase == NULL) return; // Não deveria ocorrer nunca.
    fase->dec_char_count();
}

void Jogo::render() {

    clear_to_color(tela->getTela(), BRANCO);

    // Desenha na tela os codigos fonte
    Fase *fase = fases.get_fase(fase_atual);
    if (fase != NULL) {
        CharacterIterator *ci = fase->iterate_source();
        while (ci->has_next()) {
            Character *character = ci->next();
            if (!character->is_valid()) continue;

            int cx = (character->get_x() * 8) + 35;
            int cy = (character->get_y() * 20) + 30;
            if (character->is_visible()) {
                textprintf_ex(tela->getTela(), font, cx, cy, AZUL, -1, "%c", character->get_c());
            } else {
                rectfill(tela->getTela(), cx, cy, cx + 8, cy + 8, makecol(224, 224, 224));
            }
        }
        delete ci;
    }

    std::list<Personagem*>::iterator it_personagens;
    for (it_personagens = personagens.begin(); it_personagens != personagens.end(); it_personagens++) {
        Personagem *p = *it_personagens;
        if (p->is_na_mao()) continue;
        p->render(tela->getTela(), false);
    }

    desenha_linhas(tela->getTela());

    for (it_personagens = personagens.begin(); it_personagens != personagens.end(); it_personagens++) {
        Personagem *p = *it_personagens;
        if (p->is_na_mao()) continue;
        p->render(tela->getTela(), true);
        p->proximo_quadro();
    }

    mao->render(tela->getTela());

    textout_ex(tela->getTela(), font, "Press <END> to end the game!", SCREEN_W - 255, 30, MAGENTA, -1);
    textprintf_ex(tela->getTela(), font, 2, 2, VERMELHO, -1,
            "Enemies left: %d     Programmers alive: %d     Level %d     You need %d%% of code, you have %d%%",
            inimigos_vivos, amigos_vivos, fase_atual + 1, (int) (fase->get_codigo_ok() * 100), (int) (fase->get_codigo_qtd() * 100));

    if (game_over()) {
        BITMAP *go = sprites[GAME_OVER];
        masked_blit(go, tela->getTela(), 0, 0, (SCREEN_W - go->w) / 2, (SCREEN_H - go->h) / 2, go->w, go->h);
    } else if (game_win()) {
        BITMAP *go = sprites[CONGRATS];
        masked_blit(go, tela->getTela(), 0, 0, (SCREEN_W - go->w) / 2, (SCREEN_H - go->h) / 2, go->w, go->h);
    }

    if (debug_bmp != NULL) {
        masked_blit(debug_bmp, tela->getTela(), 0, 0, 0, 0, debug_bmp->w, debug_bmp->h);
        clear_to_color(debug_bmp, MAGENTA);
    }

    blit(tela->getTela(), screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
}

BITMAP *Jogo::sprite(int numero) {
    BITMAP *b = sprites[numero];
    if (b == NULL) debug("PAU!");
    return b;
}

void Jogo::play_som(int numero, int freq) {
    SAMPLE *b = sons[numero];
    if (b == NULL) debug("PAU!");
    play_sample(b, 255, 128, freq, 0);
}

void Jogo::remove(Personagem *p) {
    char str[50];
    sprintf(str, "Destruido personagem %p do tipo %d.", p, p->get_tipo());
    debug(str);

    if (p->inimigo()) inimigos_vivos--;
    if (p->amigo()) amigos_vivos--;

    personagens.remove(p);
    matar_no_fim_da_fase.remove(p);
    mortos.push_front(p);
}

void Jogo::add(Personagem *p) {
    char str[50];
    sprintf(str, "Novo personagem %p do tipo %d.", p, p->get_tipo());
    debug(str);

    if (p->inimigo()) inimigos_vivos++;
    if (p->amigo()) amigos_vivos++;
    personagens.push_front(p);
}

void Jogo::add_nesta_fase(Personagem *p) {
    add(p);
    matar_no_fim_da_fase.push_front(p);
}

std::list<Personagem *> *Jogo::get_personagens() {
    return &personagens;
}

void Jogo::personagens_na_area(int x, int y, int larg, int alt, std::list<Personagem *>* lista, Filtro *f) {
    int idx = 0;
    int x2 = x + larg - 1;
    int y2 = y + alt - 1;
    
    std::list<Personagem *>::iterator it;
    for (it = personagens.begin(); it != personagens.end(); it++) {
        Personagem *p = (*it);
        if (p->is_na_mao()) continue;

        int o_x1 = p->get_x();
        int o_y1 = p->get_y();
        int o_x2 = o_x1 + p->get_largura() - 1;
        int o_y2 = o_y1 + p->get_altura() - 1;
        bool colide_x = colide(x, x2, o_x1, o_x2);
        bool colide_y = colide(y, y2, o_y1, o_y2);

        /*textprintf_ex(debug_bmp, font, 0, 20 * idx + 10, PRETO, -1,
                "%d: (x=%d-%d, y=%d-%d) com %p (x=%d-%d, y=%d-%d).",
                idx, x, x2, y, y2, *it, o_x1, o_x2, o_y1, o_y2);
        textprintf_ex(debug_bmp, font, 0, 20 * idx + 18, PRETO, -1,
                "%d: %s em x e %s em y.",
                idx, colide_x ? "colide" : "nao colide", colide_y ? "colide" : "nao colide");*/

        if (colide_x && colide_y && (f == NULL || f->aceita(p))) lista->push_back(p);
        idx++;
    }

    /*textprintf_ex(debug_bmp, font, 0, 20 * idx + 10, PRETO, -1,
            "Colisoes: %d.", lista->size());*/
}

void Jogo::unselect() {
    mao->unselect();
}

bool Jogo::game_over() {
    if (this->amigos_vivos == 0) return true;
    Fase *atual = fases.get_fase(fase_atual);
    return atual != NULL && !atual->is_codigo_ok();
}

bool Jogo::game_win() {
    return game_end;
}