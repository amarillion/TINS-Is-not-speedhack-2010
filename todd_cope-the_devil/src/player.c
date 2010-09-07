#include "t3f/t3f.h"
#include "main.h"
#include "enemy.h"

void player_fire_shot(float angle)
{
	int i;
	
	for(i = 0; i < GAME_MAX_PLAYER_SHOTS; i++)
	{
		if(!player_shot[i].active)
		{
			player_shot[i].x = player.x + 8;
			player_shot[i].y = player.y + 8;
			player_shot[i].angle = angle;
			player_shot[i].vx = cos(angle) * 12.0;
			player_shot[i].vy = sin(angle) * 12.0;
			player_shot[i].active = true;
			break;
		}
	}
}

void player_lose_life(void)
{
	lives--;
	al_play_sample(sample[SAMPLE_DIE], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
	if(lives <= 0)
	{
		game_state = GAME_STATE_OVER;
		player.active = false;
		al_play_sample(sample[SAMPLE_GAME_OVER], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
	}
	else
	{
//		clear_enemies();
		game_state = GAME_STATE_DIE;
		die_timer = 120;
		player.active = false;
	}
}

void player_logic(void)
{
	bool fire = false;
	float fire_angle;
	
	if(player.active)
	{
		/* move player */
		player.vx = 0;
		player.vy = 0;
		if(t3f_key[ALLEGRO_KEY_LEFT])
		{
			player.vx = -player.speed;
			player.vy = 0;
			player.angle = ALLEGRO_PI;
		}
		if(t3f_key[ALLEGRO_KEY_UP])
		{
			player.vx = 0;
			player.vy = -player.speed;
			player.angle = ALLEGRO_PI * 1.5;
		}
		if(t3f_key[ALLEGRO_KEY_RIGHT])
		{
			player.vx = player.speed;
			player.vy = 0;
			player.angle = 0;
		}
		if(t3f_key[ALLEGRO_KEY_DOWN])
		{
			player.vx = 0;
			player.vy = player.speed;
			player.angle = ALLEGRO_PI * 0.5;
		}
		if(t3f_key[ALLEGRO_KEY_LEFT] && t3f_key[ALLEGRO_KEY_UP])
		{
			player.vx = cos(1.25 * ALLEGRO_PI) * player.speed;
			player.vy = sin(1.25 * ALLEGRO_PI) * player.speed;
			player.angle = ALLEGRO_PI * 1.25;
		}
		if(t3f_key[ALLEGRO_KEY_RIGHT] && t3f_key[ALLEGRO_KEY_UP])
		{
			player.vx = cos(1.75 * ALLEGRO_PI) * player.speed;
			player.vy = sin(1.75 * ALLEGRO_PI) * player.speed;
			player.angle = ALLEGRO_PI * 1.75;
		}
		if(t3f_key[ALLEGRO_KEY_LEFT] && t3f_key[ALLEGRO_KEY_DOWN])
		{
			player.vx = cos(0.75 * ALLEGRO_PI) * player.speed;
			player.vy = sin(0.75 * ALLEGRO_PI) * player.speed;
			player.angle = ALLEGRO_PI * 0.75;
		}
		if(t3f_key[ALLEGRO_KEY_RIGHT] && t3f_key[ALLEGRO_KEY_DOWN])
		{
			player.vx = cos(0.25 * ALLEGRO_PI) * player.speed;
			player.vy = sin(0.25 * ALLEGRO_PI) * player.speed;
			player.angle = ALLEGRO_PI * 0.25;
		}
		player.x += player.vx;
		if(player.x < 0)
		{
			player.x = 0;
		}
		if(player.x > 640 - 32)
		{
			player.x = 640 - 32;
		}
		player.y += player.vy;
		if(player.y < 0)
		{
			player.y = 0;
		}
		if(player.y > 480 - 32)
		{
			player.y = 480 - 32;
		}
		t3f_move_collision_object_xy(player.object, player.x, player.y);
		
		/* fire */
		if(player.var > 0)
		{
			player.var--;
		}
		else
		{
			if(t3f_key[ALLEGRO_KEY_A])
			{
				fire = true;
				fire_angle = ALLEGRO_PI;
			}
			if(t3f_key[ALLEGRO_KEY_W])
			{
				fire = true;
				fire_angle = ALLEGRO_PI * 1.5;
			}
			if(t3f_key[ALLEGRO_KEY_D])
			{
				fire = true;
				fire_angle = 0;
			}
			if(t3f_key[ALLEGRO_KEY_S])
			{
				fire = true;
				fire_angle = ALLEGRO_PI * 0.5;
			}
			if(t3f_key[ALLEGRO_KEY_A] && t3f_key[ALLEGRO_KEY_W])
			{
				fire = true;
				fire_angle = ALLEGRO_PI * 1.25;
			}
			if(t3f_key[ALLEGRO_KEY_D] && t3f_key[ALLEGRO_KEY_W])
			{
				fire = true;
				fire_angle = ALLEGRO_PI * 1.75;
			}
			if(t3f_key[ALLEGRO_KEY_A] && t3f_key[ALLEGRO_KEY_S])
			{
				fire = true;
				fire_angle = ALLEGRO_PI * 0.75;
			}
			if(t3f_key[ALLEGRO_KEY_D] && t3f_key[ALLEGRO_KEY_S])
			{
				fire = true;
				fire_angle = ALLEGRO_PI * 0.25;
			}
			if(fire)
			{
				switch(weapon)
				{
					case 0:
					{
						player_fire_shot(fire_angle);
						break;
					}
					case 1:
					{
						player_fire_shot(fire_angle - 0.2);
						player_fire_shot(fire_angle + 0.2);
						break;
					}
					case 2:
					{
						player_fire_shot(fire_angle);
						player_fire_shot(fire_angle - 0.2);
						player_fire_shot(fire_angle + 0.2);
						break;
					}
				}
				player.var = 6;
				al_play_sample(sample[SAMPLE_SHOOT], 0.5, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			}
		}
		
		player.tick++;
	}
}

void player_render(void)
{
	if(player.active)
	{
		t3f_draw_rotated_animation(animation[ANIMATION_PLAYER], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), player.tick, 16, 16, player.x + 16, player.y + 16, player.z, player.angle, 0);
	}
}
