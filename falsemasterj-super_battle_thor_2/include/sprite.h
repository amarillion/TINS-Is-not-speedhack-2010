/* Super Battle Thor II, TINS 2010
 * Copyright (C) 2010  Johnathan Roatch
 *
 * The full copyright notice can be found in main.c
 */

#pragma once
#ifndef SPRITE_H
#define SPRITE_H

#include <allegro5/allegro.h>

#define NUM_OF_SPRITE_TYPES 10

enum sprite_type
{
  SPRITE_MJOLLNIR,
  SPRITE_FIGHT,
  SPRITE_THOR,
  SPRITE_GIANT,
  SPRITE_NIDHOGG,
  SPRITE_RAN,
  SPRITE_GRENDEL,
  SPRITE_FAFNIR,
  SPRITE_LOKI,
  SPRITE_FENRIR
};

/*
  struct sprite_prop
  {
    int x, y;
    int hp;
    bool is_bobing;
    float sine_amplitude;
    float sine_frequency;
  } sprite_start[NUM_OF_SPRITE_TYPES];
*/

ALLEGRO_BITMAP *make_from_mask (ALLEGRO_BITMAP * color,
                                ALLEGRO_BITMAP * mask);

#endif /* SPRITE_H */

