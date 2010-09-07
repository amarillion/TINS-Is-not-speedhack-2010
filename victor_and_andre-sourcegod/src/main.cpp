/*
 * TINS 2010 - The source god
 * Authors: Victor Williams Stafusa da Silva
 *          André Luiz Pereira Álvares
 */
#include "game.h"
#include "test.h"

#define TITLE "The source god - TINS 2010"
#define MODE GFX_AUTODETECT_WINDOWED

volatile int logic_time = 0, close_game = 0;
static void logic_timer() { logic_time++; }
END_OF_FUNCTION(logic_timer)
static void close_callback() { close_game = 1; }

int sinal(int x) {
    return x > 0 ? 1 : x < 0 ? -1 : 0;
}

int limitar(int valor, int min, int max) {
    return valor > max ? max : valor < min ? min : valor;
}

bool colide(int a1, int a2, int b1, int b2) {
    return (a1 <= b1 && b1 <= a2) || (a1 <= b2 && b2 <= a2) ||
           (b1 <= a1 && a1 <= b2) || (b1 <= a2 && a2 <= b2);
}

int main(int argc, char **argv) {
    debug_clear();

#if RUN_TEST
    if (argc != 1) {
        if (argc != 2) {
            printf("Wrong, too many params.");
            return -1;
        }
        pathfind_test();
        readsource_test();
        return 0;
    }
#endif

    debug("Iniciando.");
    srand(time(NULL));
    set_uformat(U_UTF8);
    allegro_init();
    install_keyboard();
    install_mouse();
    install_timer();
    install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);

    int colors = desktop_color_depth();
    set_color_depth(colors == 0 ? 32 : colors);
    set_gfx_mode(MODE, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);
    set_window_title(TITLE);
    set_close_button_callback(close_callback);
    set_volume(255, 160);
    LOCK_VARIABLE(logic_time);
    LOCK_FUNCTION(logic_timer);
    install_int_ex(logic_timer, BPS_TO_TIMER(30));

    int tick = 0;

    Ambiente *j = new MenuInicial();

    while (!key[KEY_ESC] && !close_game) {
        while (tick < logic_time) {
            j->do_logic();
            if (logic_time - tick > 3) logic_time = tick - 3;
            tick++;
        }
        j->render();
    }

    delete j;

    debug("Terminando.");
    return 0;
}
END_OF_MAIN()