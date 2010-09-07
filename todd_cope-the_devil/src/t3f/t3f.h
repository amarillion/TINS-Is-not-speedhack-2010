#ifndef T3F_H
#define T3F_H

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include <stdio.h>
#include <math.h>

#define T3F_USE_KEYBOARD    1
#define T3F_USE_MOUSE       2
#define T3F_USE_JOYSTICK    4
#define T3F_USE_SOUND       8
#define T3F_USE_FULLSCREEN 16
#define T3F_RESIZABLE      32
#define T3F_FORCE_ASPECT   64
#define T3F_USE_CONSOLE   128
#define T3F_DEFAULT (T3F_USE_KEYBOARD | T3F_USE_SOUND)

#define T3F_KEY_BUFFER_MAX 256
#define T3F_KEY_BUFFER_FORCE_LOWER 1
#define T3F_KEY_BUFFER_FORCE_UPPER 2

#define T3F_MAX_JOYSTICKS 16

#define T3F_MAX_STACK     16

extern int t3f_virtual_display_width;
extern int t3f_virtual_display_height;

extern bool t3f_key[ALLEGRO_KEY_MAX];
extern bool t3f_quit;
extern int t3f_flags;

extern int t3f_mouse_x;
extern int t3f_mouse_y;
extern int t3f_mouse_z;
extern int t3f_mouse_dx;
extern int t3f_mouse_dy;
extern int t3f_mouse_dz;
extern bool t3f_mouse_button[16];

extern ALLEGRO_JOYSTICK * t3f_joystick[T3F_MAX_JOYSTICKS];
extern ALLEGRO_JOYSTICK_STATE t3f_joystick_state[T3F_MAX_JOYSTICKS];
extern ALLEGRO_DISPLAY * t3f_display;
extern ALLEGRO_TIMER * t3f_timer;
extern ALLEGRO_CONFIG * t3f_config;
extern ALLEGRO_PATH * t3f_data_path;
extern ALLEGRO_PATH * t3f_config_path;

extern ALLEGRO_TRANSFORM t3f_base_transform;
extern ALLEGRO_TRANSFORM t3f_current_transform;

int t3f_initialize(int w, int h, double fps, char * name, void (*logic_proc)(), void (*render_proc)(), int flags);
int t3f_set_gfx_mode(int w, int h, int flags);
void t3f_exit(void);
void t3f_run(void);

float t3f_distance(float x1, float y1, float x2, float y2);
double t3f_drand(void);

void t3f_clear_keys(void);
bool t3f_add_key(char key);
char t3f_read_key(int flags);

bool t3f_push_state(int flags);
bool t3f_pop_state(void);

int t3f_get_joystick_number(ALLEGRO_JOYSTICK * jp);
float t3f_fread_float(ALLEGRO_FILE * fp);
int t3f_fwrite_float(ALLEGRO_FILE * fp, float f);

ALLEGRO_FONT * t3f_load_font(const char * fn);
ALLEGRO_FILE * t3f_open_file(ALLEGRO_PATH * pp, const char * fn, const char * m);
unsigned long t3f_checksum_file(const char * fn);
bool t3f_copy_file(const char * src, const char * dest);
void t3f_setup_directories(ALLEGRO_PATH * final);
const char * t3f_get_filename(ALLEGRO_PATH * path, const char * fn);

#endif
