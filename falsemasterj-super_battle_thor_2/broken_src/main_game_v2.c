/* Super Battle Thor II, TINS 2010
 * Copyright (C) 2010  Johnathan Roatch
 *
 * The full copyright notice can be found in main.c
 */

#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "global_vars.h"
#include "main_game.h"

enum arrow_types
{
  ARROW_NONE,
  ARROW_UP,
  ARROW_DOWN,
  ARROW_LEFT,
  ARROW_RIGHT,
  ARROW_SPACE
} current_key;

#define NUM_OF_LEVELS 10

enum level_types
{
  LEVEL_TITLE,
  LEVEL_1,
  LEVEL_2,
  LEVEL_3,
  LEVEL_4,
  LEVEL_5,
  LEVEL_6,
  LEVEL_7,
  LEVEL_GAME_OVER,
  LEVEL_YOU_WIN
} current_level;

enum entry_types
{
  ENTRY_NONE,
  ENTRY_TRANS,
  ENTRY_FROM_LEFT,
  ENTRY_FROM_RIGHT,
  ENTRY_FROM_ABOVE,
  ENTRY_FROM_WATER,
  ENTRY_FADE_IN,
  ENTRY_FENRIR,
};

bool fade_into[NUM_OF_LEVELS] = { false, true, false, false, false,
  true, true, true, true, true
};

enum background_types level_background[NUM_OF_LEVELS] =
{
  BACKGROUND_TITLE,
  BACKGROUND_1,
  BACKGROUND_1,
  BACKGROUND_1,
  BACKGROUND_1,
  BACKGROUND_5,
  BACKGROUND_6,
  BACKGROUND_BLACK,
  BACKGROUND_GAME_OVER,
  BACKGROUND_YOU_WIN
};

int level_scroll[NUM_OF_LEVELS] = { 0, 0, 294, 575, 850, 0, 0, 0, 0, 0 };

enum sound_types level_bg_music[NUM_OF_LEVELS] =
{
  SOUND_TITLESCREEN_LOOP,
  SOUND_BATTLE_LOOP,
  SOUND_BATTLE_LOOP,
  SOUND_BATTLE_LOOP,
  SOUND_BATTLE_LOOP,
  SOUND_DRAGON_LOOP,
  SOUND_BATTLE_LOOP,
  SOUND_YOU_ARE_DEAD,
  SOUND_TITLESCREEN_LOOP,
  SOUND_TITLESCREEN_LOOP
};

struct sprite_pos
{
  enum sprite_type type;
  int x, y;
  int sine_amp;
  enum entry_types entry;
};

struct sprite_pos player_sprite[NUM_OF_LEVELS] = {
  {SPRITE_MJOLLNIR, -58, 81, 0, ENTRY_NONE},
  {SPRITE_THOR, 14, 81, 0, ENTRY_FROM_LEFT},
  {SPRITE_THOR, 10, 84, 0, ENTRY_TRANS},
  {SPRITE_THOR, 22, 86, 0, ENTRY_TRANS},
  {SPRITE_THOR, 13, 83, 0, ENTRY_TRANS},
  {SPRITE_THOR, 2, 87, 0, ENTRY_NONE},
  {SPRITE_THOR, 15, 83, 0, ENTRY_FROM_LEFT},
  {SPRITE_THOR, 20, 48, 0, ENTRY_FADE_IN},
  {SPRITE_MJOLLNIR, -58, 81, 0, ENTRY_NONE},
  {SPRITE_MJOLLNIR, -58, 81, 0, ENTRY_NONE}
};

struct sprite_pos enemy_sprite[NUM_OF_LEVELS] = {
  {SPRITE_MJOLLNIR, 270, 64, 0, ENTRY_NONE},
  {SPRITE_GIANT, 158, 64, 0, ENTRY_FROM_RIGHT},
  {SPRITE_NIDHOGG, 156, 38, 0, ENTRY_FROM_ABOVE},
  {SPRITE_RAN, 133, 58, 0, ENTRY_FROM_WATER},
  {SPRITE_GRENDEL, 142, 66, 0, ENTRY_FROM_RIGHT},
  {SPRITE_FAFNIR, 68, 56, 0, ENTRY_FADE_IN},
  {SPRITE_LOKI, 156, 13, 10, ENTRY_FROM_RIGHT},
  {SPRITE_FENRIR, 55, -30, 34, ENTRY_FENRIR},
  {SPRITE_MJOLLNIR, 270, 64, 0, ENTRY_NONE},
  {SPRITE_MJOLLNIR, 270, 64, 0, ENTRY_NONE}
};

bool screen_update;
bool show_fight;

void
run_main_game ()
{
  int i;

  screen_update = true;
  current_level = LEVEL_TITLE;

  while (!global.quit)
    {
      if (screen_update)
        {
          update_now ();
        }

      al_wait_for_event (global.queue, NULL);
      event_handleing ();
    }
}

void
update_now ()
{
  al_draw_bitmap (global.backgrounds[level_background[current_level]],
                  level_scroll[current_level], 0, 0);
  
  screen_update = false;
}

void
event_handleing ()
{
  while (al_get_next_event (global.queue, &global.event))
    {
      if (global.event.type == ALLEGRO_EVENT_DISPLAY_EXPOSE
          || global.event.type == ALLEGRO_EVENT_DISPLAY_FOUND
          || global.event.type == ALLEGRO_EVENT_DISPLAY_SWITCH_IN)
        {
          screen_update = true;
        }

      if (global.event.type == ALLEGRO_EVENT_DISPLAY_CLOSE
          || (global.event.type == ALLEGRO_EVENT_KEY_DOWN
              && global.event.keyboard.keycode == ALLEGRO_KEY_ESCAPE))
        {
          global.quit = true;
        }

      if (global.event.type == ALLEGRO_EVENT_KEY_DOWN)
        {
          if (global.event.keyboard.keycode == ALLEGRO_KEY_UP)
            {
              current_key = ARROW_UP;
            }
          else if (global.event.keyboard.keycode == ALLEGRO_KEY_DOWN)
            {
              current_key = ARROW_DOWN;
            }
          else if (global.event.keyboard.keycode == ALLEGRO_KEY_LEFT)
            {
              current_key = ARROW_LEFT;
            }
          else if (global.event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
            {
              current_key = ARROW_RIGHT;
            }
          else if (global.event.keyboard.keycode == ALLEGRO_KEY_SPACE)
            {
              current_key = ARROW_SPACE;
            }
        }

      if (global.event.type == ALLEGRO_EVENT_TIMER)
        {
          main_logic ();
        }
    }
}

void
main_logic ()
{



}

