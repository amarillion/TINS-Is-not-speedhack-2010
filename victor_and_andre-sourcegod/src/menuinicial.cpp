/*
 * TINS 2010 - The source god
 * Authors: Victor Williams Stafusa da Silva
 *          André Luiz Pereira Álvares
 */
#include "game.h"

MenuInicial::MenuInicial() {
    this->j = NULL;
    this->tela = new Tela();
    this->fontcolor = makecol(0, 0, 255);
    this->srccode = get_source_by_line("readme.txt", 0);
    show_mouse(NULL);
}

MenuInicial::~MenuInicial() {
   delete tela;
   delete music;
   delete srccode;
}

void MenuInicial::do_logic() {
    if (key[KEY_END]) {
        delete this->j;
        this->j = NULL;
    }

    if (this->j != NULL) {
        this->j->do_logic();
    } else if (key[KEY_ENTER]) {
        debug("Iniciando o jogo.");
        this->j = new Jogo();

        stop_midi();
        delete this->music;
        this->music = NULL;
    } else if (this->music == NULL) {
        stop_midi();
        this->music = load_midi("media/mid/menu.mid");
        play_midi(this->music, -1);
    }
};

void MenuInicial::render() {
    if (this->j != NULL) {
        this->j->render();
    } else {
        tela->render();

        CharacterIterator *ci = new CharacterIterator(srccode);
        while (ci->has_next()) {
            Character *character = ci->next();
            if (!character->is_valid()) continue;

            int cx = (character->get_x() * 8) + 35;
            int cy = (character->get_y() * 20) + 30;
            textprintf_ex(tela->getTela(), font, cx, cy, AZUL, -1, "%c", character->get_c());
        }
        delete ci;

        /*textout_ex(tela->getTela(), font, "Authors: ", 35, 70, this->fontcolor, -1);
        textout_ex(tela->getTela(), font, "Victor Williams Stafusa da Silva", 110, 70, this->fontcolor, -1);
        textout_ex(tela->getTela(), font, "Andre Luiz Pereira Alvares", 110, 90, this->fontcolor, -1);

        textout_ex(tela->getTela(), font, "Press <ENTER> to start the game!", 280, 290, this->fontcolor, -1);*/

        show_mouse(tela->getTela());

        blit(tela->getTela(), screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    }
};