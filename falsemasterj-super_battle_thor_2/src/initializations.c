/* Super Battle Thor II, TINS 2010
 * Copyright (C) 2010  Johnathan Roatch
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * email: jroatch@gmail.com
 */

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include "global_vars.h"
#include "initializations.h"
#include "sprite.h"

bool
initialize ()
{
  ALLEGRO_BITMAP *temp_color_bitmap;
  ALLEGRO_BITMAP *temp_mask_bitmap;

  global.quit = false;
  al_set_app_name ("Super Battle Thor II");
  if (!al_init ())
    {
      fputs ("Error: Could not start allegro.\n", stderr);
      return false;
    }

  al_set_new_display_option (ALLEGRO_VSYNC, 1, ALLEGRO_SUGGEST);
  al_set_new_display_flags (ALLEGRO_GENERATE_EXPOSE_EVENTS);
  global.display = al_create_display (256, 192);
  if (!global.display)
    {
      fputs ("Error: Allegro could not create a display of 256x192 pixels.\n",
             stderr);
      return false;
    }

  if (!al_install_keyboard ())
    {
      fputs ("Allegro could not initialize the keyboard system.\n", stderr);
      return false;
    }

  global.timer = al_create_timer (ALLEGRO_BPS_TO_SECS (60));
  if (!global.timer)
    {
      fputs ("Allegro could not create a 60hrz timer.\n", stderr);
      return false;
    }

  global.queue = al_create_event_queue ();
  if (!global.queue)
    {
      fputs ("Allegro could not create an event queue.\n", stderr);
      return false;
    }
  al_register_event_source (global.queue, al_get_keyboard_event_source ());
  al_register_event_source (global.queue,
                            al_get_display_event_source (global.display));
  al_register_event_source (global.queue,
                            al_get_timer_event_source (global.timer));

  if (!al_init_image_addon ())
    {
      fputs ("Allegro could not initialize the image loading system.\n",
             stderr);
      return false;
    }

  if (!al_install_audio ())
    {
      fputs ("Allegro could not initialize the audio system.\n", stderr);
      return false;
    }
  al_reserve_samples (16);

  if (!al_init_acodec_addon ())
    {
      fputs ("Allegro could not initialize the audio file loading system.\n",
             stderr);
      return false;
    }

  al_init_font_addon ();

  /* load files */
  global.path = al_get_standard_path (ALLEGRO_PROGRAM_PATH);
  al_append_path_component (global.path, "media");

  al_set_path_filename (global.path, "proggy_tiny.png");
  global.programing_font =
    al_load_bitmap_font (al_path_cstr (global.path, ALLEGRO_NATIVE_PATH_SEP));
  if (!global.programing_font)
    {
      fputs ("Allegro could not load proggy_tiny.png.\n", stderr);
      return false;
    }

  al_set_path_filename (global.path, "mjollnir.png");
  global.sprite_bitmap[SPRITE_THOR] =
    al_load_bitmap (al_path_cstr (global.path, ALLEGRO_NATIVE_PATH_SEP));
  if (!global.sprite_bitmap[SPRITE_THOR])
    {
      fputs ("Allegro could not load mjollnir.png.\n", stderr);
      return false;
    }

  al_set_path_filename (global.path, "arrows.png");
  global.arrow_bitmap =
    al_load_bitmap (al_path_cstr (global.path, ALLEGRO_NATIVE_PATH_SEP));
  if (!global.arrow_bitmap)
    {
      fputs ("Allegro could not load arrows.png.\n", stderr);
      return false;
    }

  al_set_path_filename (global.path, "background_1_2_3_4.jpg");
  global.backgrounds[BACKGROUND_1] =
    al_load_bitmap (al_path_cstr (global.path, ALLEGRO_NATIVE_PATH_SEP));
  if (!global.backgrounds[BACKGROUND_1])
    {
      fputs ("Allegro could not load background_1_2_3_4.jpg.\n", stderr);
      return false;
    }

  al_set_path_filename (global.path, "background_5.jpg");
  global.backgrounds[BACKGROUND_5] =
    al_load_bitmap (al_path_cstr (global.path, ALLEGRO_NATIVE_PATH_SEP));
  if (!global.backgrounds[BACKGROUND_5])
    {
      fputs ("Allegro could not load background_5.jpg.\n", stderr);
      return false;
    }

  al_set_path_filename (global.path, "background_6.jpg");
  global.backgrounds[BACKGROUND_6] =
    al_load_bitmap (al_path_cstr (global.path, ALLEGRO_NATIVE_PATH_SEP));
  if (!global.backgrounds[BACKGROUND_6])
    {
      fputs ("Allegro could not load background_6.jpg.\n", stderr);
      return false;
    }

  al_set_path_filename (global.path, "title_screen.jpg");
  global.backgrounds[BACKGROUND_TITLE] =
    al_load_bitmap (al_path_cstr (global.path, ALLEGRO_NATIVE_PATH_SEP));
  if (!global.backgrounds[BACKGROUND_TITLE])
    {
      fputs ("Allegro could not load title_screen.jpg.\n", stderr);
      return false;
    }

  al_set_path_filename (global.path, "game_over.jpg");
  global.backgrounds[BACKGROUND_GAME_OVER] =
    al_load_bitmap (al_path_cstr (global.path, ALLEGRO_NATIVE_PATH_SEP));
  if (!global.backgrounds[BACKGROUND_GAME_OVER])
    {
      fputs ("Allegro could not load game_over.jpg.\n", stderr);
      return false;
    }

  al_set_path_filename (global.path, "you_win.jpg");
  global.backgrounds[BACKGROUND_YOU_WIN] =
    al_load_bitmap (al_path_cstr (global.path, ALLEGRO_NATIVE_PATH_SEP));
  if (!global.backgrounds[BACKGROUND_YOU_WIN])
    {
      fputs ("Allegro could not load you_win.jpg.\n", stderr);
      return false;
    }

  global.backgrounds[BACKGROUND_BLACK] = al_create_bitmap (256, 192);
  if (!global.backgrounds[BACKGROUND_BLACK])
    {
      fputs ("Allegro could not make a 256x192 bitmap :o", stderr);
      return false;
    }
  al_set_target_bitmap (global.backgrounds[BACKGROUND_BLACK]);
  al_clear_to_color (al_map_rgb (0, 0, 0));
  al_set_target_backbuffer (global.display);

/* start copy paste code here becasue I'm lazy */
/*------------------------------------------------------------------------*/
  al_set_path_filename (global.path, "0-thor.jpg");
  temp_color_bitmap =
    al_load_bitmap (al_path_cstr (global.path, ALLEGRO_NATIVE_PATH_SEP));
  if (!temp_color_bitmap)
    {
      fputs ("Allegro could not load 0-thor.jpg.\n", stderr);
      return false;
    }

  al_set_path_filename (global.path, "0-thor_mask.jpg");
  temp_mask_bitmap =
    al_load_bitmap (al_path_cstr (global.path, ALLEGRO_NATIVE_PATH_SEP));
  if (!temp_mask_bitmap)
    {
      fputs ("Allegro could not load 0-thor_mask.jpg.\n", stderr);
      return false;
    }

  global.sprite_bitmap[SPRITE_THOR] =
    make_from_mask (temp_color_bitmap, temp_mask_bitmap);
  al_destroy_bitmap (temp_color_bitmap);
  al_destroy_bitmap (temp_mask_bitmap);
/*------------------------------------------------------------------------*/
  al_set_path_filename (global.path, "1-giant.jpg");
  temp_color_bitmap =
    al_load_bitmap (al_path_cstr (global.path, ALLEGRO_NATIVE_PATH_SEP));
  if (!temp_color_bitmap)
    {
      fputs ("Allegro could not load 1-giant.jpg.\n", stderr);
      return false;
    }

  al_set_path_filename (global.path, "1-giant_mask.jpg");
  temp_mask_bitmap =
    al_load_bitmap (al_path_cstr (global.path, ALLEGRO_NATIVE_PATH_SEP));
  if (!temp_mask_bitmap)
    {
      fputs ("Allegro could not load 1-giant_mask.jpg.\n", stderr);
      return false;
    }

  global.sprite_bitmap[SPRITE_GIANT] =
    make_from_mask (temp_color_bitmap, temp_mask_bitmap);
  al_destroy_bitmap (temp_color_bitmap);
  al_destroy_bitmap (temp_mask_bitmap);
/*------------------------------------------------------------------------*/
  al_set_path_filename (global.path, "2-nidhogg.jpg");
  temp_color_bitmap =
    al_load_bitmap (al_path_cstr (global.path, ALLEGRO_NATIVE_PATH_SEP));
  if (!temp_color_bitmap)
    {
      fputs ("Allegro could not load 2-nidhogg.jpg.\n", stderr);
      return false;
    }

  al_set_path_filename (global.path, "2-nidhogg_mask.jpg");
  temp_mask_bitmap =
    al_load_bitmap (al_path_cstr (global.path, ALLEGRO_NATIVE_PATH_SEP));
  if (!temp_mask_bitmap)
    {
      fputs ("Allegro could not load 2-nidhogg_mask.jpg.\n", stderr);
      return false;
    }

  global.sprite_bitmap[SPRITE_NIDHOGG] =
    make_from_mask (temp_color_bitmap, temp_mask_bitmap);
  al_destroy_bitmap (temp_color_bitmap);
  al_destroy_bitmap (temp_mask_bitmap);
/*------------------------------------------------------------------------*/
  al_set_path_filename (global.path, "3-ran.jpg");
  temp_color_bitmap =
    al_load_bitmap (al_path_cstr (global.path, ALLEGRO_NATIVE_PATH_SEP));
  if (!temp_color_bitmap)
    {
      fputs ("Allegro could not load 3-ran.jpg.\n", stderr);
      return false;
    }

  al_set_path_filename (global.path, "3-ran_mask.jpg");
  temp_mask_bitmap =
    al_load_bitmap (al_path_cstr (global.path, ALLEGRO_NATIVE_PATH_SEP));
  if (!temp_mask_bitmap)
    {
      fputs ("Allegro could not load 3-ran_mask.jpg.\n", stderr);
      return false;
    }

  global.sprite_bitmap[SPRITE_RAN] =
    make_from_mask (temp_color_bitmap, temp_mask_bitmap);
  al_destroy_bitmap (temp_color_bitmap);
  al_destroy_bitmap (temp_mask_bitmap);
/*------------------------------------------------------------------------*/
  al_set_path_filename (global.path, "4-grendel.jpg");
  temp_color_bitmap =
    al_load_bitmap (al_path_cstr (global.path, ALLEGRO_NATIVE_PATH_SEP));
  if (!temp_color_bitmap)
    {
      fputs ("Allegro could not load 4-grendel.jpg.\n", stderr);
      return false;
    }

  al_set_path_filename (global.path, "4-grendel_mask.jpg");
  temp_mask_bitmap =
    al_load_bitmap (al_path_cstr (global.path, ALLEGRO_NATIVE_PATH_SEP));
  if (!temp_mask_bitmap)
    {
      fputs ("Allegro could not load 4-grendel_mask.jpg.\n", stderr);
      return false;
    }

  global.sprite_bitmap[SPRITE_GRENDEL] =
    make_from_mask (temp_color_bitmap, temp_mask_bitmap);
  al_destroy_bitmap (temp_color_bitmap);
  al_destroy_bitmap (temp_mask_bitmap);
/*------------------------------------------------------------------------*/
  al_set_path_filename (global.path, "5-fafnir.jpg");
  temp_color_bitmap =
    al_load_bitmap (al_path_cstr (global.path, ALLEGRO_NATIVE_PATH_SEP));
  if (!temp_color_bitmap)
    {
      fputs ("Allegro could not load 5-fafnir.jpg.\n", stderr);
      return false;
    }

  al_set_path_filename (global.path, "5-fafnir_mask.jpg");
  temp_mask_bitmap =
    al_load_bitmap (al_path_cstr (global.path, ALLEGRO_NATIVE_PATH_SEP));
  if (!temp_mask_bitmap)
    {
      fputs ("Allegro could not load 5-fafnir_mask.jpg.\n", stderr);
      return false;
    }

  global.sprite_bitmap[SPRITE_FAFNIR] =
    make_from_mask (temp_color_bitmap, temp_mask_bitmap);
  al_destroy_bitmap (temp_color_bitmap);
  al_destroy_bitmap (temp_mask_bitmap);
/*------------------------------------------------------------------------*/
  al_set_path_filename (global.path, "6-loki.jpg");
  temp_color_bitmap =
    al_load_bitmap (al_path_cstr (global.path, ALLEGRO_NATIVE_PATH_SEP));
  if (!temp_color_bitmap)
    {
      fputs ("Allegro could not load 6-loki.jpg.\n", stderr);
      return false;
    }

  al_set_path_filename (global.path, "6-loki_mask.jpg");
  temp_mask_bitmap =
    al_load_bitmap (al_path_cstr (global.path, ALLEGRO_NATIVE_PATH_SEP));
  if (!temp_mask_bitmap)
    {
      fputs ("Allegro could not load 6-loki_mask.jpg.\n", stderr);
      return false;
    }

  global.sprite_bitmap[SPRITE_LOKI] =
    make_from_mask (temp_color_bitmap, temp_mask_bitmap);
  al_destroy_bitmap (temp_color_bitmap);
  al_destroy_bitmap (temp_mask_bitmap);
/*------------------------------------------------------------------------*/
  al_set_path_filename (global.path, "7-fenrir.jpg");
  temp_color_bitmap =
    al_load_bitmap (al_path_cstr (global.path, ALLEGRO_NATIVE_PATH_SEP));
  if (!temp_color_bitmap)
    {
      fputs ("Allegro could not load 7-fenrir.jpg.\n", stderr);
      return false;
    }

  al_set_path_filename (global.path, "7-fenrir_mask.jpg");
  temp_mask_bitmap =
    al_load_bitmap (al_path_cstr (global.path, ALLEGRO_NATIVE_PATH_SEP));
  if (!temp_mask_bitmap)
    {
      fputs ("Allegro could not load 7-fenrir_mask.jpg.\n", stderr);
      return false;
    }

  global.sprite_bitmap[SPRITE_FENRIR] =
    make_from_mask (temp_color_bitmap, temp_mask_bitmap);
  al_destroy_bitmap (temp_color_bitmap);
  al_destroy_bitmap (temp_mask_bitmap);
/*------------------------------------------------------------------------*/
  al_set_path_filename (global.path, "fight.jpg");
  temp_color_bitmap =
    al_load_bitmap (al_path_cstr (global.path, ALLEGRO_NATIVE_PATH_SEP));
  if (!temp_color_bitmap)
    {
      fputs ("Allegro could not load fight.jpg.\n", stderr);
      return false;
    }

  al_set_path_filename (global.path, "fight_mask.jpg");
  temp_mask_bitmap =
    al_load_bitmap (al_path_cstr (global.path, ALLEGRO_NATIVE_PATH_SEP));
  if (!temp_mask_bitmap)
    {
      fputs ("Allegro could not load fight_mask.jpg.\n", stderr);
      return false;
    }

  global.sprite_bitmap[SPRITE_FIGHT] =
    make_from_mask (temp_color_bitmap, temp_mask_bitmap);
  al_destroy_bitmap (temp_color_bitmap);
  al_destroy_bitmap (temp_mask_bitmap);
/*------------------------------------------------------------------------*/
/* loading sounds here (still copy paste) */
  al_set_path_filename (global.path, "titlescreen_loop.ogg");
  global.sounds[SOUND_TITLESCREEN_LOOP] =
    al_load_sample (al_path_cstr (global.path, ALLEGRO_NATIVE_PATH_SEP));
  if (!global.sounds[SOUND_TITLESCREEN_LOOP])
    {
      fputs ("Allegro could not load titlescreen_loop.ogg.\n", stderr);
      return false;
    }

  al_set_path_filename (global.path, "begin_battle.ogg");
  global.sounds[SOUND_BEGIN_BATTLE] =
    al_load_sample (al_path_cstr (global.path, ALLEGRO_NATIVE_PATH_SEP));
  if (!global.sounds[SOUND_BEGIN_BATTLE])
    {
      fputs ("Allegro could not load begin_battle.ogg.\n", stderr);
      return false;
    }

  al_set_path_filename (global.path, "battle_loop.ogg");
  global.sounds[SOUND_BATTLE_LOOP] =
    al_load_sample (al_path_cstr (global.path, ALLEGRO_NATIVE_PATH_SEP));
  if (!global.sounds[SOUND_BATTLE_LOOP])
    {
      fputs ("Allegro could not load battle_loop.ogg.\n", stderr);
      return false;
    }

  al_set_path_filename (global.path, "dragon_loop.ogg");
  global.sounds[SOUND_DRAGON_LOOP] =
    al_load_sample (al_path_cstr (global.path, ALLEGRO_NATIVE_PATH_SEP));
  if (!global.sounds[SOUND_DRAGON_LOOP])
    {
      fputs ("Allegro could not load dragon_loop.ogg.\n", stderr);
      return false;
    }

  al_set_path_filename (global.path, "hit_1.ogg");
  global.sounds[SOUND_HIT_1] =
    al_load_sample (al_path_cstr (global.path, ALLEGRO_NATIVE_PATH_SEP));
  if (!global.sounds[SOUND_HIT_1])
    {
      fputs ("Allegro could not load hit_1.ogg.\n", stderr);
      return false;
    }

  al_set_path_filename (global.path, "hit_2.ogg");
  global.sounds[SOUND_HIT_2] =
    al_load_sample (al_path_cstr (global.path, ALLEGRO_NATIVE_PATH_SEP));
  if (!global.sounds[SOUND_HIT_2])
    {
      fputs ("Allegro could not load hit_2.ogg.\n", stderr);
      return false;
    }

  al_set_path_filename (global.path, "laugh_1.ogg");
  global.sounds[SOUND_LAUGH_1] =
    al_load_sample (al_path_cstr (global.path, ALLEGRO_NATIVE_PATH_SEP));
  if (!global.sounds[SOUND_LAUGH_1])
    {
      fputs ("Allegro could not load laugh_1.ogg.\n", stderr);
      return false;
    }

  al_set_path_filename (global.path, "laugh_2.ogg");
  global.sounds[SOUND_LAUGH_2] =
    al_load_sample (al_path_cstr (global.path, ALLEGRO_NATIVE_PATH_SEP));
  if (!global.sounds[SOUND_LAUGH_2])
    {
      fputs ("Allegro could not load laugh_2.ogg.\n", stderr);
      return false;
    }

  al_set_path_filename (global.path, "laugh_3.ogg");
  global.sounds[SOUND_LAUGH_3] =
    al_load_sample (al_path_cstr (global.path, ALLEGRO_NATIVE_PATH_SEP));
  if (!global.sounds[SOUND_LAUGH_3])
    {
      fputs ("Allegro could not load laugh_3.ogg.\n", stderr);
      return false;
    }

  al_set_path_filename (global.path, "laugh_4.ogg");
  global.sounds[SOUND_LAUGH_4] =
    al_load_sample (al_path_cstr (global.path, ALLEGRO_NATIVE_PATH_SEP));
  if (!global.sounds[SOUND_LAUGH_4])
    {
      fputs ("Allegro could not load laugh_4.ogg.\n", stderr);
      return false;
    }

  al_set_path_filename (global.path, "laugh_5.ogg");
  global.sounds[SOUND_LAUGH_5] =
    al_load_sample (al_path_cstr (global.path, ALLEGRO_NATIVE_PATH_SEP));
  if (!global.sounds[SOUND_LAUGH_5])
    {
      fputs ("Allegro could not load laugh_5.ogg.\n", stderr);
      return false;
    }

  al_set_path_filename (global.path, "laugh_6.ogg");
  global.sounds[SOUND_LAUGH_6] =
    al_load_sample (al_path_cstr (global.path, ALLEGRO_NATIVE_PATH_SEP));
  if (!global.sounds[SOUND_LAUGH_6])
    {
      fputs ("Allegro could not load laugh_6.ogg.\n", stderr);
      return false;
    }

  al_set_path_filename (global.path, "killed_baddie.ogg");
  global.sounds[SOUND_KILLED_BADDIE] =
    al_load_sample (al_path_cstr (global.path, ALLEGRO_NATIVE_PATH_SEP));
  if (!global.sounds[SOUND_KILLED_BADDIE])
    {
      fputs ("Allegro could not load killed_baddie.ogg.\n", stderr);
      return false;
    }

  al_set_path_filename (global.path, "got_hit.ogg");
  global.sounds[SOUND_GOT_HIT] =
    al_load_sample (al_path_cstr (global.path, ALLEGRO_NATIVE_PATH_SEP));
  if (!global.sounds[SOUND_GOT_HIT])
    {
      fputs ("Allegro could not load got_hit.ogg.\n", stderr);
      return false;
    }

  al_set_path_filename (global.path, "you_are_dead.ogg");
  global.sounds[SOUND_YOU_ARE_DEAD] =
    al_load_sample (al_path_cstr (global.path, ALLEGRO_NATIVE_PATH_SEP));
  if (!global.sounds[SOUND_YOU_ARE_DEAD])
    {
      fputs ("Allegro could not you_are_dead.ogg.\n", stderr);
      return false;
    }

  global.music = NULL;

  al_destroy_path (global.path);

  return true;
}


bool
deinitialize ()
{
  int i;

  al_destroy_font (global.programing_font);
  al_destroy_event_queue (global.queue);
  al_destroy_display (global.display);
  al_destroy_bitmap (global.arrow_bitmap);


  for (i = 0; i < NUM_OF_SPRITE_TYPES; i++)
    {
      al_destroy_bitmap (global.sprite_bitmap[i]);
    }

  for (i = 0; i < NUM_OF_BACKGROUNDS; i++)
    {
      al_destroy_bitmap (global.backgrounds[i]);
    }

  for (i = 0; i < NUM_OF_SOUNDS; i++)
    {
      al_destroy_sample (global.sounds[i]);
    }

  /*if (global.music)
     {
     al_drain_audio_stream (global.music);
     al_destroy_audio_stream (global.music);
     } */
  al_shutdown_image_addon ();
  al_uninstall_audio ();
  al_uninstall_keyboard ();
  return true;
}

