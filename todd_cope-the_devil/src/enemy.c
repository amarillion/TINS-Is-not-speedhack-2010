#include <math.h>
#include "t3f/t3f.h"
#include "t3f/animation.h"
#include "main.h"
#include "player.h"

void clear_enemies(void)
{
	int i;
	
	for(i = 0; i < GAME_MAX_ENEMIES; i++)
	{
		enemy[i].active = false;
	}
}

int count_enemies(void)
{
	int i, c = 0;
	
	for(i = 0; i < GAME_MAX_ENEMIES; i++)
	{
		if(enemy[i].active)
		{
			c++;
		}
	}
	return c;
}

void enemy_fire_shot(float x, float y, float angle)
{
	int i;
	
	for(i = 0; i < GAME_MAX_ENEMY_SHOTS; i++)
	{
		if(!enemy_shot[i].active)
		{
			enemy_shot[i].x = x;
			enemy_shot[i].y = y;
			enemy_shot[i].angle = angle;
			enemy_shot[i].vx = cos(angle) * 6.0;
			enemy_shot[i].vy = sin(angle) * 6.0;
			enemy_shot[i].active = true;
			break;
		}
	}
}

void enemy_spawn_logic_normal(void)
{
	int i;
	int r;
	
	if(rand() % 1000 < level.demon_rate)
	{
		for(i = 0; i < GAME_MAX_ENEMIES; i++)
		{
			if(!enemy[i].active)
			{
				r = rand() % enemy_spawners;
				enemy[i].x = enemy_spawner[r].x;
				enemy[i].y = enemy_spawner[r].y;
				enemy[i].type = ENEMY_TYPE_DEMON;
				enemy[i].speed = 2.0 + t3f_drand() * (level.demon_speed - 2.0);
				enemy[i].active = true;
				break;
			}
		}
	}
	if(rand() % 1000 < level.archdemon_rate)
	{
		for(i = 0; i < GAME_MAX_ENEMIES; i++)
		{
			if(!enemy[i].active)
			{
				r = rand() % enemy_spawners;
				enemy[i].x = enemy_spawner[r].x;
				enemy[i].y = enemy_spawner[r].y;
				enemy[i].type = ENEMY_TYPE_ARCHDEMON;
				enemy[i].speed = (2.0 + t3f_drand() * (level.demon_speed - 2.0)) / 2.0;
				enemy[i].var = 60;
				enemy[i].active = true;
				break;
			}
		}
	}
}

void enemy_spawn_logic_easy(void)
{
	int i;
	int r;
	
	if(rand() % 1500 < level.demon_rate)
	{
		for(i = 0; i < GAME_MAX_ENEMIES; i++)
		{
			if(!enemy[i].active)
			{
				r = rand() % enemy_spawners;
				enemy[i].x = enemy_spawner[r].x;
				enemy[i].y = enemy_spawner[r].y;
				enemy[i].type = ENEMY_TYPE_DEMON;
				enemy[i].speed = 2.0 + t3f_drand() * (level.demon_speed - 2.0);
				enemy[i].active = true;
				break;
			}
		}
	}
	if(rand() % 1500 < level.archdemon_rate)
	{
		for(i = 0; i < GAME_MAX_ENEMIES; i++)
		{
			if(!enemy[i].active)
			{
				r = rand() % enemy_spawners;
				enemy[i].x = enemy_spawner[r].x;
				enemy[i].y = enemy_spawner[r].y;
				enemy[i].type = ENEMY_TYPE_ARCHDEMON;
				enemy[i].speed = (2.0 + t3f_drand() * (level.demon_speed - 2.0)) / 2.0;
				enemy[i].var = 60;
				enemy[i].active = true;
				break;
			}
		}
	}
}

void enemy_logic(void)
{
	int i;
	float target_angle;

	for(i = 0; i < GAME_MAX_ENEMIES; i++)
	{
		if(enemy[i].active)
		{
			switch(enemy[i].type)
			{
				case ENEMY_TYPE_DEMON:
				{
					if(player.active)
					{
						target_angle = atan2(enemy[i].y - player.y, enemy[i].x - player.x);
						enemy[i].angle = target_angle + ALLEGRO_PI;
						enemy[i].vx = cos(enemy[i].angle) * enemy[i].speed;
						enemy[i].vy = sin(enemy[i].angle) * enemy[i].speed;
					}
					else
					{
						enemy[i].vx *= 1.01;
						enemy[i].vy *= 1.01;
					}
					enemy[i].x += enemy[i].vx;
					enemy[i].y += enemy[i].vy;
					t3f_move_collision_object_xy(enemy[i].object, enemy[i].x, enemy[i].y);
					if(player.active && t3f_check_object_collision(enemy[i].object, player.object))
					{
						player_lose_life();
						enemy[i].active = false;
					}
					if(enemy[i].x < -34 || enemy[i].x > 642 || enemy[i].y < -34 || enemy[i].y > 482)
					{
						enemy[i].active = false;
					}
					break;
				}
				case ENEMY_TYPE_ARCHDEMON:
				{
					if(player.active)
					{
						target_angle = atan2(enemy[i].y - player.y, enemy[i].x - player.x);
						enemy[i].angle = target_angle + ALLEGRO_PI;
						enemy[i].vx = cos(enemy[i].angle) * enemy[i].speed;
						enemy[i].vy = sin(enemy[i].angle) * enemy[i].speed;
					}
					else
					{
						enemy[i].vx *= 1.01;
						enemy[i].vy *= 1.01;
					}
					enemy[i].x += enemy[i].vx;
					enemy[i].y += enemy[i].vy;
					t3f_move_collision_object_xy(enemy[i].object, enemy[i].x, enemy[i].y);
					if(player.active && t3f_check_object_collision(enemy[i].object, player.object))
					{
						player_lose_life();
						enemy[i].active = false;
					}
					if(enemy[i].var > 0)
					{
						enemy[i].var--;
					}
					else
					{
						if(player.active && rand() % 1000 < level.archdemon_fire_rate)
						{
							enemy_fire_shot(enemy[i].x + 8, enemy[i].y + 8, enemy[i].angle);
							enemy[i].var = 60;
						}
					}
					if(enemy[i].x < -34 || enemy[i].x > 642 || enemy[i].y < -34 || enemy[i].y > 482)
					{
						enemy[i].active = false;
					}
					break;
				}
			}
			enemy[i].tick++;
		}
	}
}

void enemy_render(void)
{
	int i;

	for(i = 0; i < GAME_MAX_ENEMIES; i++)
	{
		if(enemy[i].active)
		{
			switch(enemy[i].type)
			{
				case ENEMY_TYPE_DEMON:
				{
					t3f_draw_rotated_animation(animation[ANIMATION_DEMON], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), enemy[i].tick, 16, 16, enemy[i].x + 16, enemy[i].y + 16, 0, enemy[i].angle, 0);
					break;
				}
				case ENEMY_TYPE_ARCHDEMON:
				{
					t3f_draw_rotated_animation(animation[ANIMATION_ARCHDEMON], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), enemy[i].tick, 16, 16, enemy[i].x + 16, enemy[i].y + 16, 0, enemy[i].angle, 0);
//					t3f_draw_animation(animation[ANIMATION_ARCHDEMON], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), enemy[i].tick, enemy[i].x, enemy[i].y, 0, 0);
					break;
				}
			}
		}
	}
}
