#include <stdio.h>
#include "t3f/t3f.h"
#include "t3f/animation.h"
#include "t3f/sound.h"
#include "t3f/music.h"
#include "main.h"
#include "game.h"
#include "title.h"
#include "player.h"
#include "enemy.h"
#include "projectile.h"
#include "powerup.h"

bool game_init(void)
{
	int i;
	
	/* create spawners */
	enemy_spawners = 0;
	for(i = 0; i < (640 + 32 + 32) / 32; i++)
	{
		enemy_spawner[enemy_spawners].x = -32 + i * 32;
		enemy_spawner[enemy_spawners].y = -32;
		enemy_spawners++;
		enemy_spawner[enemy_spawners].x = -32 + i * 32;
		enemy_spawner[enemy_spawners].y = 480;
		enemy_spawners++;
	}
	for(i = 0; i < (480 + 32 + 32) / 32; i++)
	{
		enemy_spawner[enemy_spawners].x = -32;
		enemy_spawner[enemy_spawners].y = -32 + i * 32;
		enemy_spawners++;
		enemy_spawner[enemy_spawners].x = 640;
		enemy_spawner[enemy_spawners].y = -32 + i * 32;
		enemy_spawners++;
	}
	
	/* create game objects */
	player.object = t3f_create_collision_object(8, 8, 16, 16, 32, 32, 0);
	for(i = 0; i < GAME_MAX_ENEMIES; i++)
	{
		enemy[i].object = t3f_create_collision_object(8, 8, 16, 16, 32, 32, 0);
		enemy[i].active = false;
	}
	for(i = 0; i < GAME_MAX_PLAYER_SHOTS; i++)
	{
		player_shot[i].object = t3f_create_collision_object(0, 0, 16, 16, 32, 32, 0);
		player_shot[i].active = false;
	}
	for(i = 0; i < GAME_MAX_ENEMY_SHOTS; i++)
	{
		enemy_shot[i].object = t3f_create_collision_object(0, 0, 16, 16, 32, 32, 0);
		enemy_shot[i].active = false;
	}
	for(i = 0; i < GAME_MAX_POWERUPS; i++)
	{
		powerup[i].object = t3f_create_collision_object(0, 0, 16, 16, 32, 32, 0);
		powerup[i].active = false;
	}
	
	/* set game up */
	player.x = 304;
	player.y = 224;
	player.z = 0;
	player.speed = 4.0;
	player.active = true;
	state = STATE_GAME;
	score = 0;
	if(finale_mode)
	{
		current_level = 9;
	}
	else
	{
		current_level = 0;
	}
	game_load_level(&level, "data/levels.dat", current_level);
	multiplier = 1;
	if(konami_mode)
	{
		lives = 99;
	}
	else
	{
		lives = 3;
	}
	if(fire_power)
	{
		weapon = 2;
	}
	else
	{
		weapon = 0;
	}
	state_ticks = 0;
	got_high_score = false;
	game_state = GAME_STATE_PLAY;
	t3f_play_music("data/game.xm", 0.0, 0.0);
	return true;
}

void game_exit(void)
{
	int i;
	char text[256] = {0};
	
	/* destroy game objects */
	t3f_destroy_collision_object(player.object);
	for(i = 0; i < GAME_MAX_ENEMIES; i++)
	{
		t3f_destroy_collision_object(enemy[i].object);
	}
	for(i = 0; i < GAME_MAX_PLAYER_SHOTS; i++)
	{
		t3f_destroy_collision_object(player_shot[i].object);
	}
	for(i = 0; i < GAME_MAX_ENEMY_SHOTS; i++)
	{
		t3f_destroy_collision_object(enemy_shot[i].object);
	}
	for(i = 0; i < GAME_MAX_POWERUPS; i++)
	{
		t3f_destroy_collision_object(powerup[i].object);
	}
	sprintf(text, "%d", high_score);
	al_set_config_value(t3f_config, "Save Data", "High Score", text);
	current_menu = TITLE_MENU_MAIN;
	state = STATE_TITLE;
	al_show_mouse_cursor(t3f_display);
	t3f_play_music("data/title.xm", 0.0, 0.0);
}

bool game_load_level(LEVEL_DATA * lp, const char * fn, int level)
{
	ALLEGRO_CONFIG * config;
	char section[256] = {0};
	const char * val;
	
	config = al_load_config_file(fn);
	if(config)
	{
		sprintf(section, "Level %d", level);
		val = al_get_config_value(config, section, "Demon Rate");
		if(val)
		{
			lp->demon_rate = atoi(val);
		}
		val = al_get_config_value(config, section, "Demon Speed");
		if(val)
		{
			lp->demon_speed = atof(val);
		}
		val = al_get_config_value(config, section, "Demon Fire Rate");
		if(val)
		{
			lp->demon_fire_rate = atoi(val);
		}
		val = al_get_config_value(config, section, "Arch-Demon Rate");
		if(val)
		{
			lp->archdemon_rate = atoi(val);
		}
		val = al_get_config_value(config, section, "Arch-Demon Fire Rate");
		if(val)
		{
			lp->archdemon_fire_rate = atoi(val);
		}
		val = al_get_config_value(config, section, "Fireball Rate");
		if(val)
		{
			lp->fireball_rate = atoi(val);
		}
		val = al_get_config_value(config, section, "Nuke Rate");
		if(val)
		{
			lp->nuke_rate = atoi(val);
		}
		al_destroy_config(config);
		return true;
	}
	return false;
}

void game_logic(void)
{
	if(t3f_key[ALLEGRO_KEY_ESCAPE])
	{
		game_exit();
		t3f_key[ALLEGRO_KEY_ESCAPE] = 0;
		return;
	}
	switch(game_state)
	{
		case GAME_STATE_PLAY:
		{
			enemy_spawn_logic();
			player_logic();
			enemy_logic();
			projectile_logic();
			powerup_logic();
			state_ticks++;
			if(state_ticks % 1800 == 0)
			{
				current_level++;
				if(current_level == 2)
				{
					weapon++;
					al_play_sample(sample[SAMPLE_TWIN_SHOT], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
				if(current_level == 6)
				{
					weapon++;
					al_play_sample(sample[SAMPLE_TRIPLE_SHOT], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
				if(weapon > 2)
				{
					weapon = 2;
				}
				if(current_level > 10)
				{
					current_level = 10;
				}
				game_load_level(&level, "data/levels.dat", current_level);
			}
			if(current_level >= 10 && count_enemies() <= 0)
			{
				game_state = GAME_STATE_END;
			}
			break;
		}
		case GAME_STATE_DIE:
		{
			projectile_logic();
			enemy_logic();
			powerup_logic();
			if(count_enemies() <= 0)
			{
				player.x = 304;
				player.y = 224;
				player.speed = 4.0;
				player.active = true;
				multiplier = 1;
				game_state = GAME_STATE_PLAY;
			}
			break;
		}
		case GAME_STATE_OVER:
		{
			projectile_logic();
			enemy_logic();
			powerup_logic();
			if(count_enemies() <= 0)
			{
				game_exit();
			}
			break;
		}
		case GAME_STATE_END:
		{
			projectile_logic();
			enemy_logic();
			powerup_logic();
			player.active = true;
			player.angle += 0.05;
			player.z -= 8.0;
			if(player.z <= -640)
			{
				game_exit();
				ending_cinema->position = 0;
				state = STATE_ENDING;
			}
			break;
		}
	}
	if(score > high_score)
	{
		high_score = score;
		if(!got_high_score)
		{
			al_play_sample(sample[SAMPLE_HIGH_SCORE], 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			got_high_score = true;
		}
	}
}

void game_render(void)
{
	al_clear_to_color(level_color[current_level]);
	player_render();
	enemy_render();
	projectile_render();
	powerup_render();
	al_draw_textf(font[FONT_GAME], al_map_rgba_f(0.0, 0.0, 0.0, 0.8), 320 + 2, 0 + 2, ALLEGRO_ALIGN_CENTRE, "High Score: %06d", high_score);
	al_draw_textf(font[FONT_GAME], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), 320, 0, ALLEGRO_ALIGN_CENTRE, "High Score: %06d", high_score);
	al_draw_textf(font[FONT_GAME], al_map_rgba_f(0.0, 0.0, 0.0, 0.8), 4 + 2, 0 + 2, 0, "Score: %06d", score);
	al_draw_textf(font[FONT_GAME], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), 4, 0, 0, "Score: %06d", score);
	al_draw_textf(font[FONT_GAME], al_map_rgba_f(0.0, 0.0, 0.0, 0.8), 4 + 2, 16 + 2, 0, "Multiplier: %d", multiplier);
	al_draw_textf(font[FONT_GAME], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), 4, 16, 0, "Multiplier: %d", multiplier);
	al_draw_textf(font[FONT_GAME], al_map_rgba_f(0.0, 0.0, 0.0, 0.8), 562 + 2, 0 + 2, 0, "Level: %d", current_level);
	al_draw_textf(font[FONT_GAME], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), 562, 0, 0, "Level: %d", current_level);
	al_draw_textf(font[FONT_GAME], al_map_rgba_f(0.0, 0.0, 0.0, 0.8), 562 + 2, 16 + 2, 0, "Lives: %d", lives);
	al_draw_textf(font[FONT_GAME], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), 562, 16, 0, "Lives: %d", lives);
}
