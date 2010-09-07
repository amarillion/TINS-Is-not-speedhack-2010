/* Super Battle Thor II, TINS 2010
 * Copyright (C) 2010  Johnathan Roatch
 *
 * The full copyright notice can be found in main.c
 */

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include "global_vars.h"
#include "intro_screen.h"

void
run_into_screen ()
{
  ALLEGRO_USTR *text_buffer[NUM_OF_LINES];
  ALLEGRO_FILE *file;
  ALLEGRO_USTR *line;
  int screen_update = true;
  bool intro_exit = false;
  bool wait_a_little_bit = true;
  int lines_out = 5;
  int y, lh;

  text_buffer[0] = al_ustr_new ("Johnathan Roatch: code");
  text_buffer[1] = al_ustr_new ("Benjamin Kroll: art");
  text_buffer[2] = al_ustr_new ("Brandon Ballschmidt: music");
  text_buffer[3] = al_ustr_new ("Pedro Gontijo: Final boss music");
  text_buffer[4] = al_ustr_new ("       Presents...");

  for (y = 5; y < NUM_OF_LINES; y++)
    {
      text_buffer[y] = al_ustr_new (" ");
    }

  al_clear_to_color (al_map_rgb (0, 0, 0));
  al_flip_display ();

  global.path = al_get_standard_path (ALLEGRO_PROGRAM_PATH);
  al_append_path_component (global.path, "src");
  al_set_path_filename (global.path, "initializations.c");

  file = al_fopen (al_path_cstr (global.path, ALLEGRO_NATIVE_PATH_SEP), "r");

  al_set_timer_speed (global.timer, 3);

  al_start_timer (global.timer);

  while (!global.quit && !intro_exit)
    {
      if (screen_update)
        {
          lh = al_get_font_line_height (global.programing_font);
          al_clear_to_color (al_map_rgb (0, 0, 0));
          al_hold_bitmap_drawing (true);
          for (y = 0; y < NUM_OF_LINES; y++)
            {
              al_draw_ustr (global.programing_font,
                            al_map_rgb_f (1.0, 1.0, 1.0), TEXT_OFFSET_X,
                            y * lh + TEXT_OFFSET_Y, ALLEGRO_ALIGN_LEFT,
                            text_buffer[y]);
            }

          al_hold_bitmap_drawing (false);
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

          if (global.event.type == ALLEGRO_EVENT_KEY_DOWN
              && global.event.keyboard.keycode == ALLEGRO_KEY_ENTER)
            {
              intro_exit = true;
            }

          if (global.event.type == ALLEGRO_EVENT_TIMER)
            {
              if (wait_a_little_bit)
                {
                  al_set_timer_speed (global.timer,
                                      ALLEGRO_BPS_TO_SECS (100));
                  wait_a_little_bit = false;
                }
              else if (al_feof (file))
                {
                  intro_exit = true;
                }
              else
                {
                  line = al_fget_ustr (file);
                  al_ustr_rtrim_ws (line);
                  if (lines_out >= NUM_OF_LINES)
                    {
                      for (y = 0; y < NUM_OF_LINES - 1; y++)
                        {
                          al_ustr_assign (text_buffer[y], text_buffer[y + 1]);
                        }
                      lines_out = NUM_OF_LINES - 1;
                    }
                  al_ustr_assign (text_buffer[lines_out], line);
                  lines_out++;
                  screen_update = true;
                }
            }
        }
    }

  al_clear_to_color (al_map_rgb (0, 0, 0));
  al_flip_display ();
  al_rest (0.5);

  al_fclose (file);
  al_destroy_path (global.path);
}

