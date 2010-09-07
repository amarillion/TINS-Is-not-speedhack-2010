#ifndef T3F_SPRITE_SHEET_H
#define T3F_SPRITE_SHEET_H

#include <allegro5/allegro5.h>

#define T3F_SPRITE_SHEET_TYPE_TILES   0
#define T3F_SPRITE_SHEET_TYPE_SPRITES 1

void t3f_start_sprite_sheet(ALLEGRO_BITMAP * bp, int type);
ALLEGRO_BITMAP * t3f_add_bitmap_to_sprite_sheet(ALLEGRO_BITMAP * bp);
void t3f_finish_sprite_sheet(void);

#endif
