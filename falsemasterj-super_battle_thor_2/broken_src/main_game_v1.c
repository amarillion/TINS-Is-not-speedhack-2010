/* Super Battle Thor II, TINS 2010
 * Copyright (C) 2010  Johnathan Roatch
 *
 * The full copyright notice can be found in main.c
 */

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "global_vars.h"
#include "main_game.h"



void
run_main_game ()
{
  screen_update = true;
  music_started = false;
  current_fade = 0;

  scroll_time = 0;

  al_set_timer_speed (global.timer, ALLEGRO_BPS_TO_SECS (60));
  current_state = STATE_TITLE;
  fadeout = FADE_STOP;

  while (!global.quit)
    {
      main_logic ();

      if (screen_update)
        {
          switch (current_state)
            {
            case STATE_TITLE:
              al_draw_bitmap (global.backgrounds[BACKGROUND_TITLE], 0, 0, 0);
              break;
            case STATE_GAME_OVER:
              al_draw_bitmap (global.backgrounds[BACKGROUND_GAME_OVER], 0, 0,
                              0);
              break;
            case STATE_YOU_WIN:
              al_draw_bitmap (global.backgrounds[BACKGROUND_YOU_WIN], 0, 0,
                              0);
              break;
            case STATE_MAP_1:
            case STATE_SCROLLING_TO_MAP_1:
              al_draw_bitmap (global.backgrounds[BACKGROUND_1], 0, 0, 0);
              break;
            case STATE_MAP_2:
              al_draw_bitmap (global.backgrounds[BACKGROUND_1], -294, 0, 0);
              break;
            case STATE_MAP_3:
              al_draw_bitmap (global.backgrounds[BACKGROUND_1], -575, 0, 0);
              break;
            case STATE_MAP_4:
              al_draw_bitmap (global.backgrounds[BACKGROUND_1], -850, 0, 0);
              break;
            case STATE_MAP_5:
              al_draw_bitmap (global.backgrounds[BACKGROUND_5], 0, 0, 0);
              break;
            case STATE_MAP_6:
              al_draw_bitmap (global.backgrounds[BACKGROUND_6], 0, 0, 0);
              break;
            case STATE_MAP_7:
              al_clear_to_color (al_map_rgb (38, 26, 51));
              break;
            default:
              ;
            }

          if (current_fade > 0 && current_fade < 256)
            {
              al_draw_tinted_bitmap (global.backgrounds[BACKGROUND_BLACK],
                                     al_map_rgba (0, 0, 0, current_fade), 0,
                                     0, 0);
            }
          al_flip_display ();
          screen_update = false;
        }

      al_wait_for_event (global.queue, NULL);

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

          if (global.event.type == ALLEGRO_EVENT_TIMER)
            {
              switch (fadeout)
                {
                case FADE_STOP:
                  break;
                case FADE_OUT:
                  current_fade += 4;
                  if (current_fade >= 255)
                    {
                      fadeout = FADE_STOP;
                      current_fade = 255;
                      music_started = false;
                    }
                  screen_update = true;
                  break;
                case FADE_IN:
                  current_fade -= 4;
                  if (current_fade <= 0)
                    {
                      fadeout = FADE_STOP;
                      current_fade = 0;
                    }
                  screen_update = true;
                  break;
                default:
                  ;
                }
            }

          switch (current_state)
            {
            case STATE_TITLE:
              if (current_fade == 255)
                {
                  current_state = STATE_SCROLLING_TO_MAP_1;
                  break;
                }
            case STATE_GAME_OVER:
            case STATE_YOU_WIN:
              if (global.event.type == ALLEGRO_EVENT_KEY_DOWN)
                {
                  fadeout = FADE_OUT;
                  al_stop_samples ();
                }
              if (current_fade == 255)
                {
                  current_state = STATE_TITLE;
                  fadeout = FADE_IN;
                }
              break;
            case STATE_SCROLLING_TO_MAP_1:
              if (current_fade == 255)
                {
                  fadeout = FADE_IN;
                }
              if (global.event.type == ALLEGRO_EVENT_TIMER)
                {
                  scroll_time++;
                }
              break;
            default:
              current_state = STATE_TITLE;
              screen_update = true;
              continue;
            }
        }
    }

  if (global.music)
    {
      al_drain_audio_stream (global.music);
      al_destroy_audio_stream (global.music);
      global.music = NULL;
    }

}

void
main_logic ()
{
  if (!music_started)
    {
      switch (current_state)
        {
        case STATE_TITLE:
        case STATE_GAME_OVER:
        case STATE_YOU_WIN:
          al_play_sample (global.sounds[SOUND_TITLESCREEN_LOOP], 1.0, 0.0,
                          1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
          break;
        case STATE_MAP_1:
        case STATE_MAP_2:
        case STATE_MAP_3:
        case STATE_MAP_4:
        case STATE_MAP_6:
          al_play_sample (global.sounds[SOUND_BATTLE_LOOP], 1, 0, 1,
                          ALLEGRO_PLAYMODE_LOOP, NULL);
          break;
        case STATE_MAP_5:
          al_play_sample (global.sounds[SOUND_DRAGON_LOOP], 1, 0, 1,
                          ALLEGRO_PLAYMODE_LOOP, NULL);
          break;
        case STATE_SCROLLING_TO_MAP_1:
        case STATE_SCROLLING_TO_MAP_2:
        case STATE_SCROLLING_TO_MAP_3:
        case STATE_SCROLLING_TO_MAP_4:
          /*case STATE_SCROLLING_TO_MAP_5: */
        case STATE_SCROLLING_TO_MAP_6:
          /*case STATE_SCROLLING_TO_MAP_7: */
          al_play_sample (global.sounds[SOUND_BEGIN_BATTLE], 1, 0, 1,
                          ALLEGRO_PLAYMODE_ONCE, NULL);
          break;

        case STATE_MAP_7:
          global.path = al_get_standard_path (ALLEGRO_PROGRAM_PATH);
          al_append_path_component (global.path, "media");
          al_set_path_filename (global.path, "last_boss.xm");

          global.music =
            al_load_audio_stream (al_path_cstr
                                  (global.path, ALLEGRO_NATIVE_PATH_SEP), 4,
                                  1024);
          al_attach_audio_stream_to_mixer (global.music,
                                           al_get_default_mixer ());

          al_destroy_path (global.path);
          break;
        default:
          ;
        }
      music_started = true;
    }
}

