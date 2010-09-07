#include "t3f/t3f.h"
#include "main.h"
#include "title.h"

int logo_timer = 0;

void logo_logic(void)
{
	if(t3f_read_key(0) && logo_timer < 120)
	{
		logo_timer = 120;
	}
	logo_timer++;
	if(logo_timer > 150)
	{
		title_init();
	}
}

void logo_render(void)
{
	ALLEGRO_COLOR color;
	
	al_clear_to_color(al_map_rgba_f(0.0, 0.0, 0.0, 1.0));
	if(logo_timer < 30)
	{
		color = al_map_rgba_f(1.0, 1.0, 1.0, (float)logo_timer / 30.0);
	}
	else if(logo_timer > 120)
	{
		color = al_map_rgba_f(1.0, 1.0, 1.0, 1.0 - (float)(logo_timer - 120) / 30.0);
	}
	else
	{
		color = al_map_rgba_f(1.0, 1.0, 1.0, 1.0);
	}
	t3f_draw_animation(animation[ANIMATION_LOGO], color, logo_timer, 320 - al_get_bitmap_width(animation[ANIMATION_LOGO]->bitmap[0]) / 2, 240 - al_get_bitmap_height(animation[ANIMATION_LOGO]->bitmap[0]) / 2, 0, 0);
}

