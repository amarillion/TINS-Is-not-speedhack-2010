/* Super Battle Thor II, TINS 2010
 * Copyright (C) 2010  Johnathan Roatch
 *
 * The full copyright notice can be found in main.c
 */

#include <allegro5/allegro.h>
#include "sprite.h"

ALLEGRO_BITMAP *
make_from_mask (ALLEGRO_BITMAP * color, ALLEGRO_BITMAP * mask)
{
  ALLEGRO_BITMAP *result;
  unsigned char r, g, b, a;
  int width, height;
  int x, y;
  ALLEGRO_BITMAP *return_target;


  if (al_get_bitmap_width (color) > al_get_bitmap_width (mask))
    width = al_get_bitmap_width (color);
  else
    width = al_get_bitmap_width (mask);

  if (al_get_bitmap_height (color) > al_get_bitmap_height (mask))
    height = al_get_bitmap_height (color);
  else
    height = al_get_bitmap_height (mask);

  result = al_create_bitmap (width, height);

  if (!result)
    return result;

  al_lock_bitmap (color, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_READONLY);
  al_lock_bitmap (mask, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_READONLY);
  al_lock_bitmap (result, ALLEGRO_PIXEL_FORMAT_ANY_WITH_ALPHA,
                  ALLEGRO_LOCK_WRITEONLY);

  return_target = al_get_target_bitmap ();
  al_set_target_bitmap (result);

  for (y = 0; y < height; y++)
    {
      for (x = 0; x < width; x++)
        {
          al_unmap_rgb (al_get_pixel (color, x, y), &r, &g, &b);
          al_unmap_rgb (al_get_pixel (mask, x, y), &a, &a, &a);
          al_put_pixel (x, y, al_map_rgba (r, g, b, a));
        }
    }

  al_set_target_bitmap (return_target);

  al_unlock_bitmap (color);
  al_unlock_bitmap (mask);
  al_unlock_bitmap (result);
  return result;
}

