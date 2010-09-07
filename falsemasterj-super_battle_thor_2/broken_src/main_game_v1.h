/* Super Battle Thor II, TINS 2010
 * Copyright (C) 2010  Johnathan Roatch
 *
 * The full copyright notice can be found in main.c
 */

#pragma once
#ifndef MAIN_GAME_H
#define MAIN_GAME_H

enum game_state
{
  STATE_TITLE,
  STATE_GAME_OVER,
  STATE_YOU_WIN,
  STATE_SCROLLING_TO_MAP_1,
  STATE_MAP_1,
  STATE_SCROLLING_TO_MAP_2,
  STATE_MAP_2,
  STATE_SCROLLING_TO_MAP_3,
  STATE_MAP_3,
  STATE_SCROLLING_TO_MAP_4,
  STATE_MAP_4,
  STATE_SCROLLING_TO_MAP_5,
  STATE_MAP_5,
  STATE_SCROLLING_TO_MAP_6,
  STATE_MAP_6,
  STATE_SCROLLING_TO_MAP_7,
  STATE_MAP_7
} current_state;

enum fadeout_type
{
  FADE_STOP,
  FADE_OUT,
  FADE_IN
} fadeout;

int screen_update;
int music_started;
int current_fade;
int scroll_time;

void run_main_game ();
void main_logic ();

#endif /* MAIN_GAME_H */

