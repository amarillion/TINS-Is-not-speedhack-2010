#include <allegro5/allegro5.h>
#include <stdio.h>
#include "sprite_sheet.h"

static int t3f_ss_working_type = 0;
static int t3f_ss_working_pos_x = 0;
static int t3f_ss_working_pos_y = 0;
static int t3f_ss_working_line_height = 0;
static ALLEGRO_BITMAP * t3f_ss_working_bitmap = NULL;

void t3f_start_sprite_sheet(ALLEGRO_BITMAP * bp, int type)
{
	t3f_ss_working_bitmap = bp;
	t3f_ss_working_type = type;
	t3f_ss_working_pos_x = 0;
	t3f_ss_working_pos_y = 0;
	t3f_ss_working_line_height = 0;
}

/* fix for when you have exceeded the size of the sprite sheet */
ALLEGRO_BITMAP * t3f_add_bitmap_to_sprite_sheet(ALLEGRO_BITMAP * bp)
{
	ALLEGRO_STATE old_state;
	ALLEGRO_BITMAP * retbp = NULL;
	ALLEGRO_TRANSFORM identity_transform;
	
	if(!t3f_ss_working_bitmap || t3f_ss_working_pos_y >= al_get_bitmap_height(t3f_ss_working_bitmap))
	{
		return NULL;
	}
	
	al_store_state(&old_state, ALLEGRO_STATE_TARGET_BITMAP | ALLEGRO_STATE_BLENDER | ALLEGRO_STATE_TRANSFORM);
	al_set_target_bitmap(t3f_ss_working_bitmap);
	al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ZERO);
	al_identity_transform(&identity_transform);
	al_use_transform(&identity_transform);
	switch(t3f_ss_working_type)
	{
		case T3F_SPRITE_SHEET_TYPE_TILES:
		{
			
			/* go to next line if it doesn't fit */
			if(t3f_ss_working_pos_x + al_get_bitmap_width(bp) + 2 >= al_get_bitmap_width(t3f_ss_working_bitmap))
			{
				t3f_ss_working_pos_x = 0;
				t3f_ss_working_pos_y += t3f_ss_working_line_height;
				
				/* if it still doesn't fit, fail */
				if(t3f_ss_working_pos_y  + al_get_bitmap_height(bp) + 2 >= al_get_bitmap_height(t3f_ss_working_bitmap))
				{
					al_restore_state(&old_state);
					return NULL;
				}
			}
			
			/* need to extend edges of tiles so they don't have soft edges */
			al_draw_bitmap(bp, t3f_ss_working_pos_x, t3f_ss_working_pos_y, 0);
			al_draw_bitmap(bp, t3f_ss_working_pos_x + 2, t3f_ss_working_pos_y, 0);
			al_draw_bitmap(bp, t3f_ss_working_pos_x, t3f_ss_working_pos_y + 2, 0);
			al_draw_bitmap(bp, t3f_ss_working_pos_x + 2, t3f_ss_working_pos_y + 2, 0);
			al_draw_bitmap(bp, t3f_ss_working_pos_x + 1, t3f_ss_working_pos_y, 0);
			al_draw_bitmap(bp, t3f_ss_working_pos_x + 1, t3f_ss_working_pos_y + 2, 0);
			al_draw_bitmap(bp, t3f_ss_working_pos_x, t3f_ss_working_pos_y + 1, 0);
			al_draw_bitmap(bp, t3f_ss_working_pos_x + 2, t3f_ss_working_pos_y + 1, 0);
			al_draw_bitmap(bp, t3f_ss_working_pos_x + 1, t3f_ss_working_pos_y + 1, 0);
			
			retbp = al_create_sub_bitmap(t3f_ss_working_bitmap, t3f_ss_working_pos_x + 1, t3f_ss_working_pos_y + 1, al_get_bitmap_width(bp), al_get_bitmap_height(bp));
			
			t3f_ss_working_pos_x += al_get_bitmap_width(bp) + 2;
			if(al_get_bitmap_height(bp) + 2 > t3f_ss_working_line_height)
			{
				t3f_ss_working_line_height = al_get_bitmap_height(bp) + 2;
			}
			break;
		}
		case T3F_SPRITE_SHEET_TYPE_SPRITES:
		{
			/* go to next line if it doesn't fit */
			if(t3f_ss_working_pos_x + al_get_bitmap_width(bp) + 1 >= al_get_bitmap_width(t3f_ss_working_bitmap))
			{
				t3f_ss_working_pos_x = 0;
				t3f_ss_working_pos_y += t3f_ss_working_line_height;
				
				/* if it still doesn't fit, fail */
				if(t3f_ss_working_pos_y + al_get_bitmap_height(bp) + 1 >= al_get_bitmap_height(t3f_ss_working_bitmap))
				{
					al_restore_state(&old_state);
					return NULL;
				}
			}
			al_draw_bitmap(bp, t3f_ss_working_pos_x, t3f_ss_working_pos_y, 0);
			
			retbp = al_create_sub_bitmap(t3f_ss_working_bitmap, t3f_ss_working_pos_x, t3f_ss_working_pos_y, al_get_bitmap_width(bp), al_get_bitmap_height(bp));
			
			t3f_ss_working_pos_x += al_get_bitmap_width(bp) + 1;
			if(al_get_bitmap_height(bp) + 1 > t3f_ss_working_line_height)
			{
				t3f_ss_working_line_height = al_get_bitmap_height(bp) + 1;
			}
			break;
		}
	}
	al_restore_state(&old_state);
		
	return retbp;
}

void t3f_finish_sprite_sheet(void)
{
	t3f_ss_working_bitmap = NULL;
}
