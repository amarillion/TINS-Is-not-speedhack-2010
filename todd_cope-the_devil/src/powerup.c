#include "t3f/t3f.h"
#include "main.h"

void generate_powerup(int type, float x, float y, float vx, float vy)
{
	int i;
	
	for(i = 0; i < GAME_MAX_POWERUPS; i++)
	{
		if(!powerup[i].active)
		{
			powerup[i].type = type;
			powerup[i].x = x;
			powerup[i].y = y;
			powerup[i].vx = vx;
			powerup[i].vy = vy;
			powerup[i].var = 120;
			powerup[i].active = true;
			break;
		}
	}
}

void powerup_logic(void)
{
	int i;
	
	for(i = 0; i < GAME_MAX_POWERUPS; i++)
	{
		if(powerup[i].active)
		{
			switch(powerup[i].type)
			{
				case POWERUP_TYPE_SPIRIT:
				{
					powerup[i].var--;
					powerup[i].x += powerup[i].vx;
					powerup[i].y += powerup[i].vy;
					if(powerup[i].var <= 0)
					{
						powerup[i].active = false;
					}
					else
					{
						t3f_move_collision_object_xy(powerup[i].object, powerup[i].x, powerup[i].y);
						if(player.active && t3f_check_object_collision(powerup[i].object, player.object))
						{
							al_play_sample(sample[SAMPLE_POWERUP], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
							score += 10 * multiplier;
							multiplier_tick++;
							if(multiplier_tick >= 10)
							{
								al_play_sample(sample[SAMPLE_MULTIPLIER], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
								multiplier++;
								if(multiplier == 10)
								{
									al_play_sample(sample[SAMPLE_MAX_MULTIPLIER], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
								}
								if(multiplier > 10)
								{
									multiplier = 10;
								}
								multiplier_tick = 0;
							}
							powerup[i].active = 0;
						}
					}
					powerup[i].tick++;
					break;
				}
				case POWERUP_TYPE_UPGRADE:
				{
					break;
				}
			}
		}
	}
}

void powerup_render(void)
{
	int i;
	
	for(i = 0; i < GAME_MAX_POWERUPS; i++)
	{
		if(powerup[i].active)
		{
			switch(powerup[i].type)
			{
				case POWERUP_TYPE_SPIRIT:
				{
					t3f_draw_animation(animation[ANIMATION_SPIRIT], al_map_rgba_f(1.0, 1.0, 1.0, (float)powerup[i].var / 120.0), powerup[i].tick, powerup[i].x, powerup[i].y, 0, 0);
					break;
				}
				case POWERUP_TYPE_UPGRADE:
				{
					break;
				}
			}
		}
	}
}
