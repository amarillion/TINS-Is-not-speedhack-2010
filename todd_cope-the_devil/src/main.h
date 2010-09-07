#ifndef MAIN_H
#define MAIN_H

#include "t3f/animation.h"
#include "cinema.h"
#include "game.h"

#define STATE_INTRO  0
#define STATE_TITLE  1
#define STATE_GAME   2
#define STATE_LOGO   3
#define STATE_ENDING 4

#define MAX_ANIMATIONS        256
#define ANIMATION_PLAYER        0
#define ANIMATION_DEMON         1
#define ANIMATION_ARCHDEMON     2
#define ANIMATION_PLAYER_SHOT   3
#define ANIMATION_ENEMY_SHOT    4
#define ANIMATION_SPIRIT        5
#define ANIMATION_LOGO          6
#define ANIMATION_TITLE         7

#define MAX_FONTS               8
#define FONT_MENU               0
#define FONT_GAME               1

#define MAX_SAMPLES            16
#define SAMPLE_LEVEL_UP         0
#define SAMPLE_GAME_OVER        1
#define SAMPLE_HIGH_SCORE       2
#define SAMPLE_MAX_MULTIPLIER   3
#define SAMPLE_TWIN_SHOT        4
#define SAMPLE_TRIPLE_SHOT      5
#define SAMPLE_POWERUP          6
#define SAMPLE_SHOOT            7
#define SAMPLE_DIE              8
#define SAMPLE_HIT              9
#define SAMPLE_MULTIPLIER      10

#define ENEMY_TYPE_DEMON        0
#define ENEMY_TYPE_ARCHDEMON    1

#define POWERUP_TYPE_SPIRIT     0
#define POWERUP_TYPE_UPGRADE    1

extern int state;
extern int state_ticks;
extern T3F_ANIMATION * animation[MAX_ANIMATIONS];
extern ALLEGRO_FONT * font[MAX_FONTS];
extern ALLEGRO_SAMPLE * sample[MAX_SAMPLES];
extern CINEMA * cinema;
extern CINEMA * ending_cinema;
extern int current_menu;

/* game objects */
extern LEVEL_DATA  level;
extern GAME_ENTITY player;
extern GAME_ENTITY enemy[GAME_MAX_ENEMIES];
extern GAME_ENTITY player_shot[GAME_MAX_PLAYER_SHOTS];
extern GAME_ENTITY enemy_shot[GAME_MAX_ENEMY_SHOTS];
extern GAME_ENTITY powerup[GAME_MAX_POWERUPS];
extern GAME_ENTITY_SPAWNER enemy_spawner[GAME_MAX_ENTITY_SPAWNERS];
extern int enemy_spawners;
extern int current_level;
extern int score;
extern int high_score;
extern bool got_high_score;
extern int multiplier;
extern int multiplier_tick;
extern int weapon;
extern int game_state;
extern int die_timer;
extern int lives;
extern ALLEGRO_COLOR level_color[11];
void (*enemy_spawn_logic)();
extern bool konami_mode;
extern bool fire_power;
extern bool finale_mode;

#endif
