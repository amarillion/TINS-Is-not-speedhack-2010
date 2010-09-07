/* Super Battle Thor II, TINS 2010
 * Copyright (C) 2010  Johnathan Roatch
 *
 * The full copyright notice can be found in main.c
 */

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include "global_vars.h"
#include "main_game.h"

void
run_main_game ()
{
  int slide_num = 0;
  bool screen_update = true;


  while (!global.quit)
    {
      if (screen_update)
        {
          al_clear_to_color (al_map_rgb (0, 0, 0));
          switch (slide_num)
            {
            case 0:
              al_draw_text (global.programing_font,
                            al_map_rgb_f (1.0, 1.0, 1.0),
                            0, 0, ALLEGRO_ALIGN_LEFT,
                            "The following is a slideshow of What");
              al_draw_text (global.programing_font,
                            al_map_rgb_f (1.0, 1.0, 1.0),
                            0, 11, ALLEGRO_ALIGN_LEFT,
                            "could of been the game.");

              al_draw_text (global.programing_font,
                            al_map_rgb_f (1.0, 1.0, 1.0),
                            0, 33, ALLEGRO_ALIGN_LEFT,
                            "I apologize to my fellow teammates.");
              al_draw_text (global.programing_font,
                            al_map_rgb_f (1.0, 1.0, 1.0),
                            0, 44, ALLEGRO_ALIGN_LEFT,
                            "mabye I can make it up someday.");

              al_draw_text (global.programing_font,
                            al_map_rgb_f (1.0, 1.0, 1.0),
                            0, 66, ALLEGRO_ALIGN_LEFT,
                            "press any key to advance the slides.");
              al_draw_text (global.programing_font,
                            al_map_rgb_f (1.0, 1.0, 1.0),
                            0, 77, ALLEGRO_ALIGN_LEFT, "esc quits.");
              break;
            case 1:
              al_draw_bitmap (global.backgrounds[BACKGROUND_TITLE], 0, 0, 0);
              break;
            case 2:
              al_draw_bitmap (global.backgrounds[BACKGROUND_1], 0, 0, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_THOR], 14, 81, 0);
              break;
            case 3:
              al_draw_bitmap (global.backgrounds[BACKGROUND_1], 0, 0, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_THOR], 14, 81, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_GIANT], 158, 64, 0);
              break;
            case 4:
              al_draw_bitmap (global.backgrounds[BACKGROUND_1], 0, 0, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_THOR], 14, 81, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_GIANT], 158, 64, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_FIGHT], 66, 3, 0);
              break;
            case 5:
              al_draw_bitmap (global.backgrounds[BACKGROUND_1], 0, 0, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_THOR], 14, 81, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_GIANT], 158, 64, 0);
              al_draw_bitmap_region (global.arrow_bitmap, 0, 0, 32, 32, 8, 8,
                                     0);
              al_draw_bitmap_region (global.arrow_bitmap, 128, 0, 32, 32, 40,
                                     8, 0);
              al_draw_bitmap_region (global.arrow_bitmap, 128, 0, 32, 32, 72,
                                     8, 0);
              al_draw_bitmap_region (global.arrow_bitmap, 128, 0, 32, 32, 104,
                                     8, 0);
              break;
            case 6:
              al_draw_bitmap (global.backgrounds[BACKGROUND_1], 0, 0, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_THOR], 14, 81, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_GIANT], 158, 64, 0);
              al_draw_bitmap_region (global.arrow_bitmap, 128, 0, 32, 32, 40,
                                     8, 0);
              al_draw_bitmap_region (global.arrow_bitmap, 128, 0, 32, 32, 72,
                                     8, 0);
              al_draw_bitmap_region (global.arrow_bitmap, 128, 0, 32, 32, 104,
                                     8, 0);
              break;
            case 7:
              al_draw_bitmap (global.backgrounds[BACKGROUND_1], 0, 0, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_THOR], 14, 81, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_GIANT], 158, 64, 0);
              al_draw_bitmap_region (global.arrow_bitmap, 128, 0, 32, 32, 72,
                                     8, 0);
              al_draw_bitmap_region (global.arrow_bitmap, 128, 0, 32, 32, 104,
                                     8, 0);
              break;
            case 8:
              al_draw_bitmap (global.backgrounds[BACKGROUND_1], 0, 0, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_THOR], 14, 81, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_GIANT], 158, 64, 0);
              al_draw_bitmap_region (global.arrow_bitmap, 128, 0, 32, 32, 104,
                                     8, 0);
              break;
            case 9:
              al_draw_bitmap (global.backgrounds[BACKGROUND_1], 0, 0, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_THOR], 14, 81, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_GIANT], 158, 64, 0);
              break;
            case 10:
              al_draw_bitmap (global.backgrounds[BACKGROUND_1], 0, 0, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_THOR], 14, 81, 0);
              break;
            case 11:
              al_draw_bitmap (global.backgrounds[BACKGROUND_1], -73, 0, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_THOR], 13, 81, 0);
              break;
            case 12:
              al_draw_bitmap (global.backgrounds[BACKGROUND_1], -147, 0, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_THOR], 12, 82, 0);
              break;
            case 13:
              al_draw_bitmap (global.backgrounds[BACKGROUND_1], -294, 0, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_THOR], 10, 84, 0);
              break;
            case 14:
              al_draw_bitmap (global.backgrounds[BACKGROUND_1], -294, 0, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_THOR], 10, 84, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_NIDHOGG], 156, 38, 0);
              break;
            case 15:
              al_draw_bitmap (global.backgrounds[BACKGROUND_1], -294, 0, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_THOR], 10, 84, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_NIDHOGG], 156, 38, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_FIGHT], 66, 3, 0);
              break;
            case 16:
            case 20:
              al_draw_bitmap (global.backgrounds[BACKGROUND_1], -294, 0, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_THOR], 10, 84, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_NIDHOGG], 156, 38, 0);
              al_draw_bitmap_region (global.arrow_bitmap, 0, 0, 32, 32, 8, 8,
                                     0);
              al_draw_bitmap_region (global.arrow_bitmap, 64, 0, 32, 32, 40,
                                     8, 0);
              al_draw_bitmap_region (global.arrow_bitmap, 32, 0, 32, 32, 72,
                                     8, 0);
              al_draw_bitmap_region (global.arrow_bitmap, 96, 0, 32, 32, 104,
                                     8, 0);
              break;
            case 17:
            case 21:
              al_draw_bitmap (global.backgrounds[BACKGROUND_1], -294, 0, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_THOR], 10, 84, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_NIDHOGG], 156, 38, 0);
              al_draw_bitmap_region (global.arrow_bitmap, 64, 0, 32, 32, 40,
                                     8, 0);
              al_draw_bitmap_region (global.arrow_bitmap, 32, 0, 32, 32, 72,
                                     8, 0);
              al_draw_bitmap_region (global.arrow_bitmap, 96, 0, 32, 32, 104,
                                     8, 0);
              break;
            case 18:
            case 22:
              al_draw_bitmap (global.backgrounds[BACKGROUND_1], -294, 0, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_THOR], 10, 84, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_NIDHOGG], 156, 38, 0);
              al_draw_bitmap_region (global.arrow_bitmap, 32, 0, 32, 32, 72,
                                     8, 0);
              al_draw_bitmap_region (global.arrow_bitmap, 96, 0, 32, 32, 104,
                                     8, 0);
              break;
            case 19:
            case 23:
              al_draw_bitmap (global.backgrounds[BACKGROUND_1], -294, 0, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_THOR], 10, 84, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_NIDHOGG], 156, 38, 0);
              al_draw_bitmap_region (global.arrow_bitmap, 96, 0, 32, 32, 104,
                                     8, 0);
              break;
            case 24:
              al_draw_bitmap (global.backgrounds[BACKGROUND_1], -294, 0, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_THOR], 10, 84, 0);
              break;
            case 25:
              al_draw_bitmap (global.backgrounds[BACKGROUND_1], -388, 0, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_THOR], 14, 85, 0);
              break;
            case 26:
              al_draw_bitmap (global.backgrounds[BACKGROUND_1], -480, 0, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_THOR], 18, 85, 0);
              break;
            case 27:
              al_draw_bitmap (global.backgrounds[BACKGROUND_1], -575, 0, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_THOR], 22, 86, 0);
              break;
            case 28:
              al_draw_bitmap (global.backgrounds[BACKGROUND_1], -575, 0, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_THOR], 22, 86, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_RAN], 133, 58, 0);
              break;
            case 29:
              al_draw_bitmap (global.backgrounds[BACKGROUND_1], -575, 0, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_THOR], 22, 86, 0);
              break;
            case 30:
              al_draw_bitmap (global.backgrounds[BACKGROUND_1], -850, 0, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_THOR], 13, 83, 0);
              break;
            case 31:
              al_draw_bitmap (global.backgrounds[BACKGROUND_1], -850, 0, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_THOR], 13, 83, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_GRENDEL], 133, 58, 0);
              break;
            case 32:
              al_draw_bitmap (global.backgrounds[BACKGROUND_1], -850, 0, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_THOR], 13, 83, 0);
              break;
            case 33:
            case 45:
            case 48:
            case 53:
            case 60:
              break;
            case 34:
              al_draw_bitmap (global.backgrounds[BACKGROUND_5], 0, 0, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_THOR], 2, 87, 0);
              break;
            case 35:
              al_draw_bitmap (global.backgrounds[BACKGROUND_5], 0, 0, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_THOR], 2, 87, 0);
              al_draw_tinted_bitmap(global.sprite_bitmap[SPRITE_FAFNIR], al_map_rgba_f(1, 1, 1, 0.25), 68, 56, 0);
              break;
            case 36:
              al_draw_bitmap (global.backgrounds[BACKGROUND_5], 0, 0, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_THOR], 2, 87, 0);
              al_draw_tinted_bitmap(global.sprite_bitmap[SPRITE_FAFNIR], al_map_rgba_f(1, 1, 1, 0.50), 68, 56, 0);
              break;
            case 37:
              al_draw_bitmap (global.backgrounds[BACKGROUND_5], 0, 0, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_THOR], 2, 87, 0);
              al_draw_tinted_bitmap(global.sprite_bitmap[SPRITE_FAFNIR], al_map_rgba_f(1, 1, 1, 0.75), 68, 56, 0);
              break;
            case 38:
              al_draw_bitmap (global.backgrounds[BACKGROUND_5], 0, 0, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_THOR], 2, 87, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_FAFNIR], 68, 56, 0);
              break;
            case 39:
              al_draw_bitmap (global.backgrounds[BACKGROUND_5], 0, 0, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_THOR], 2, 87, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_FAFNIR], 68, 56, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_FIGHT], 66, 3, 0);
              break;
            case 40:
            case 41:
            case 42:
            case 43:
              al_draw_bitmap (global.backgrounds[BACKGROUND_5], 0, 0, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_THOR], 2, 87, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_FAFNIR], 68, 56, 0);
              al_draw_bitmap_region (global.arrow_bitmap, 96, 0, 32, 32, 8, 8,
                                     0);
              al_draw_bitmap_region (global.arrow_bitmap, 0, 0, 32, 32, 40,
                                     0, 0);
              al_draw_bitmap_region (global.arrow_bitmap, 64, 0, 32, 32, 72,
                                     0, 0);
              al_draw_bitmap_region (global.arrow_bitmap, 128, 0, 32, 32, 104,
                                     0, 0);
              al_draw_bitmap_region (global.arrow_bitmap, 32, 0, 32, 32, 8, 40,
                                     0);
              al_draw_bitmap_region (global.arrow_bitmap, 32, 0, 32, 32, 40,
                                     40, 0);
              al_draw_bitmap_region (global.arrow_bitmap, 0, 0, 32, 32, 72,
                                     40, 0);
              al_draw_bitmap_region (global.arrow_bitmap, 128, 0, 32, 32, 104,
                                     40, 0);
              break;
            case 44:
              al_draw_bitmap (global.backgrounds[BACKGROUND_5], 0, 0, 0);
              al_draw_rotated_bitmap(global.sprite_bitmap[SPRITE_THOR],
   26, 102, 32, 189, 5 ,0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_FAFNIR], 68, 56, 0);
              break;
            case 46:
              al_draw_bitmap (global.backgrounds[BACKGROUND_GAME_OVER], 0, 0, 0);
              break;
            case 47:
              al_draw_text (global.programing_font,
                            al_map_rgb_f (1.0, 1.0, 1.0),
                            0, 33, ALLEGRO_ALIGN_LEFT,
                            "Fast Forwarding to after the dragon.");
              break;
            case 49:
              al_draw_bitmap (global.backgrounds[BACKGROUND_6], 0, 0, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_THOR], 15, 83, 0);
              break;
            case 50:
              al_draw_bitmap (global.backgrounds[BACKGROUND_6], 0, 0, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_THOR], 15, 83, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_LOKI], 156, 13, 0);
              break;
            case 51:
              al_draw_bitmap (global.backgrounds[BACKGROUND_6], 0, 0, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_THOR], 15, 83, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_LOKI], 156, 13, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_FIGHT], 66, 3, 0);
              break;
            case 52:
              al_draw_bitmap (global.backgrounds[BACKGROUND_6], 0, 0, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_THOR], 15, 83, 0);
              break;
            case 54:
              al_clear_to_color (al_map_rgb (38, 26, 51));
              al_draw_bitmap (global.sprite_bitmap[SPRITE_THOR], 15, 83, 0);
              al_draw_bitmap (global.sprite_bitmap[SPRITE_FENRIR], 55, -30, 0);
              break;
            case 59:
              al_draw_text (global.programing_font,
                            al_map_rgb_f (1.0, 1.0, 1.0),
                            12, 77, ALLEGRO_ALIGN_LEFT,
                            "     --Johnathan Roatch.");
            case 58:
              al_draw_text (global.programing_font,
                            al_map_rgb_f (1.0, 1.0, 1.0),
                            12, 55, ALLEGRO_ALIGN_LEFT,
                            "It was fun, see ya later");
            case 57:
              al_draw_text (global.programing_font,
                            al_map_rgb_f (1.0, 1.0, 1.0),
                            12, 55, ALLEGRO_ALIGN_LEFT,
                            "It was fun,");
            case 56:
              al_draw_text (global.programing_font,
                            al_map_rgb_f (1.0, 1.0, 1.0),
                            12, 44, ALLEGRO_ALIGN_LEFT,
                            "BTW, Awesome music Pedro Gontijo");
            case 55:
              al_draw_text (global.programing_font,
                            al_map_rgb_f (1.0, 1.0, 1.0),
                            12, 33, ALLEGRO_ALIGN_LEFT,
                            "Too bad i didn't finish the game");
              break;
            case 61:
              al_draw_bitmap (global.backgrounds[BACKGROUND_YOU_WIN], 0, 0, 0);
              break;
              
            default:
              al_clear_to_color (al_map_rgb (0, 0, 0));
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

          if (global.event.type == ALLEGRO_EVENT_KEY_DOWN)
            {
              slide_num++;
              switch (slide_num)
                {
                case 0:
                  break;
                case 1:
                case 46:
                  al_play_sample (global.sounds[SOUND_TITLESCREEN_LOOP],
                                  1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
                  break;
                case 2:
                case 11:
                case 25:
                case 49:
                  al_stop_samples ();
                  al_play_sample (global.sounds[SOUND_BEGIN_BATTLE],
                                  1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
                  break;
                case 3:
                  al_play_sample (global.sounds[SOUND_LAUGH_1],
                                  1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                  break;
                case 4:
                case 15:
                case 51:
                  al_stop_samples ();
                  al_play_sample (global.sounds[SOUND_BATTLE_LOOP],
                                  1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
                  break;
                case 6:
                case 8:
                case 9:
                case 18:
                case 20:
                case 22:
                  al_play_sample (global.sounds[SOUND_HIT_1],
                                  1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                  break;
                case 7:
                case 17:
                case 19:
                case 21:
                case 23:
                  al_play_sample (global.sounds[SOUND_HIT_2],
                                  1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                  break;
                case 10:
                case 24:
                case 29:
                case 32:
                case 52:
                  al_play_sample (global.sounds[SOUND_KILLED_BADDIE],
                                  1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                  break;
                case 14:
                  al_play_sample (global.sounds[SOUND_LAUGH_2],
                                  1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                  break;
                case 28:
                  al_play_sample (global.sounds[SOUND_LAUGH_3],
                                  1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                  break;
                case 31:
                  al_play_sample (global.sounds[SOUND_LAUGH_4],
                                  1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                  break;
                case 33:
                case 45:
                case 47:
                case 53:
                  al_stop_samples ();
                  break;
                case 34:
                  al_play_sample (global.sounds[SOUND_DRAGON_LOOP],
                                  1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
                  break;
                case 38:
                  al_play_sample (global.sounds[SOUND_LAUGH_5],
                                  1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                  break;
                case 41:
                case 42:
                case 43:
                  al_play_sample (global.sounds[SOUND_GOT_HIT],
                                  1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                  break;
                case 44:
                  al_play_sample (global.sounds[SOUND_YOU_ARE_DEAD],
                                  1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                  break;
                case 50:
                  al_play_sample (global.sounds[SOUND_LAUGH_6],
                                  1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                  break;
                case 5:
                case 12:
                case 13:
                case 16:
                case 26:
                case 27:
                case 30:
                case 35:
                case 36:
                case 37:
                case 39:
                case 40:
                case 48:
                case 55:
                case 56:
                case 57:
                case 58:
                case 59:
                case 60:
                case 61:
                  break;

                case 54:
                  global.path = al_get_standard_path (ALLEGRO_PROGRAM_PATH);
                  al_append_path_component (global.path, "media");
                  al_set_path_filename (global.path, "last_boss.xm");

                  global.music = al_load_audio_stream (al_path_cstr (global.path, ALLEGRO_NATIVE_PATH_SEP), 4, 1024);
                  al_attach_audio_stream_to_mixer (global.music, al_get_default_mixer ());
                  al_destroy_path (global.path);
                  break;

                case 62:
                  if (global.music)
                  {
                    al_drain_audio_stream (global.music);
                    al_destroy_audio_stream (global.music);
                  }
                  global.quit = true;
                  break;

                default:
                  global.quit = true;
                }
              screen_update = true;
            }
        }
    }
}

