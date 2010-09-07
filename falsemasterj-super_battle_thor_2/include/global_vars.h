/* Super Battle Thor II, TINS 2010
 * Copyright (C) 2010  Johnathan Roatch
 *
 * The full copyright notice can be found in main.c
 */

#pragma once
#ifndef GLOBAL_VARS_H
#define GLOBAL_VARS_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include "sprite.h"

#define NUM_OF_BACKGROUNDS 7

enum background_types
{
  BACKGROUND_BLACK,
  BACKGROUND_1,
  BACKGROUND_5,
  BACKGROUND_6,
  BACKGROUND_TITLE,
  BACKGROUND_YOU_WIN,
  BACKGROUND_GAME_OVER
};

#define NUM_OF_SOUNDS 15
enum sound_types
{
  SOUND_TITLESCREEN_LOOP,
  SOUND_BEGIN_BATTLE,
  SOUND_BATTLE_LOOP,
  SOUND_DRAGON_LOOP,
  SOUND_HIT_1,
  SOUND_HIT_2,
  SOUND_LAUGH_1,
  SOUND_LAUGH_2,
  SOUND_LAUGH_3,
  SOUND_LAUGH_4,
  SOUND_LAUGH_5,
  SOUND_LAUGH_6,
  SOUND_KILLED_BADDIE,
  SOUND_GOT_HIT,
  SOUND_YOU_ARE_DEAD
};

/* all main allegro objects go in this global variable*/
struct aleg_objs
{
  ALLEGRO_DISPLAY *display;
  ALLEGRO_TIMER *timer;
  ALLEGRO_EVENT_QUEUE *queue;
  ALLEGRO_PATH *path;
  ALLEGRO_AUDIO_STREAM *music;

  ALLEGRO_EVENT event;

  ALLEGRO_FONT *programing_font;
  ALLEGRO_BITMAP *arrow_bitmap;
  ALLEGRO_BITMAP *backgrounds[NUM_OF_BACKGROUNDS];

  ALLEGRO_BITMAP *sprite_bitmap[NUM_OF_SPRITE_TYPES];

  ALLEGRO_SAMPLE *sounds[NUM_OF_SOUNDS];

  bool quit;

} global;

#endif /* GLOBAL_VARS_H */

