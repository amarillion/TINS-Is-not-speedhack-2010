#include "t3f/t3f.h"
#include "t3f/3d.h"
#include "t3f/hyperlink.h"
#include "t3f/sound.h"
#include "t3f/music.h"
#include "t3f/sprite_sheet.h"
#include "cinema.h"
#include "main.h"
#include "title.h"
#include "game.h"
#include "logo.h"

T3F_3D_STATE projection;
int state = STATE_LOGO;
int state_ticks = 0;
T3F_ANIMATION * animation[MAX_ANIMATIONS] = {NULL};
ALLEGRO_FONT * font[MAX_FONTS] = {NULL};
ALLEGRO_SAMPLE * sample[MAX_SAMPLES] = {NULL};
ALLEGRO_BITMAP * sprite_sheet = NULL;
CINEMA * cinema = NULL;
CINEMA * ending_cinema = NULL;
int current_menu = TITLE_MENU_MAIN;

/* game data */
LEVEL_DATA level;
GAME_ENTITY player;
GAME_ENTITY enemy[GAME_MAX_ENEMIES];
GAME_ENTITY player_shot[GAME_MAX_PLAYER_SHOTS];
GAME_ENTITY enemy_shot[GAME_MAX_ENEMY_SHOTS];
GAME_ENTITY powerup[GAME_MAX_POWERUPS];
GAME_ENTITY_SPAWNER enemy_spawner[GAME_MAX_ENTITY_SPAWNERS];
int enemy_spawners = 0;
int current_level = 0;
int score = 0;
int high_score = 10000;
bool got_high_score = false;
int multiplier = 1;
int multiplier_tick = 0;
int weapon = 0;
int game_state = GAME_STATE_PLAY;
int die_timer = 0;
int lives = 0;
ALLEGRO_COLOR level_color[11];
void (*enemy_spawn_logic)() = NULL;
bool konami_mode = false;
bool fire_power = false;
bool finale_mode = false;

void logic(void)
{
	switch(state)
	{
		case STATE_LOGO:
		{
			logo_logic();
			break;
		}
		case STATE_INTRO:
		{
			cinema_logic(cinema);
			if(cinema->position >= cinema->frames)
			{
				state = STATE_TITLE;
			}
			break;
		}
		case STATE_TITLE:
		{
			title_logic();
			break;
		}
		case STATE_GAME:
		{
			game_logic();
			break;
		}
		case STATE_ENDING:
		{
			cinema_logic(ending_cinema);
			if(ending_cinema->position >= ending_cinema->frames)
			{
				state = STATE_TITLE;
				current_menu = TITLE_MENU_MAIN;
			}
			break;
		}
	}
	t3f_poll_sound_queue();
}

void render(void)
{
	al_hold_bitmap_drawing(true);
	switch(state)
	{
		case STATE_LOGO:
		{
			logo_render();
			break;
		}
		case STATE_INTRO:
		{
			cinema_render(cinema);
			break;
		}
		case STATE_TITLE:
		{
			title_render();
			break;
		}
		case STATE_GAME:
		{
			game_render();
			break;
		}
		case STATE_ENDING:
		{
			cinema_render(ending_cinema);
			break;
		}
	}
	al_hold_bitmap_drawing(false);
}

void process_arguments(int argc, char * argv[])
{
	int i;
	
	for(i = 1; i < argc; i++)
	{
		if(!stricmp(argv[i], "-goobers"))
		{
			konami_mode = true;
		}
		if(!stricmp(argv[i], "-firepower"))
		{
			fire_power = true;
		}
		if(!stricmp(argv[i], "-finale"))
		{
			finale_mode = true;
		}
	}
}

bool initialize(int argc, char * argv[])
{
	const char * val;
	ALLEGRO_BITMAP * bp;
	
	process_arguments(argc, argv);
	if(!t3f_initialize(640, 480, 60.0, "The Devil", logic, render, T3F_DEFAULT | T3F_USE_MOUSE))
	{
		return false;
	}
	t3f_3d_set_projection(&projection, 0, 0, 640, 480, 320, 240);
	t3f_3d_select_projection(&projection);
	animation[ANIMATION_PLAYER] = t3f_load_animation_from_bitmap("data/player.png");
	if(!animation[ANIMATION_PLAYER])
	{
		return false;
	}
	animation[ANIMATION_DEMON] = t3f_load_animation_from_bitmap("data/demon.png");
	if(!animation[ANIMATION_DEMON])
	{
		return false;
	}
	animation[ANIMATION_ARCHDEMON] = t3f_load_animation_from_bitmap("data/archdemon.png");
	if(!animation[ANIMATION_ARCHDEMON])
	{
		return false;
	}
	animation[ANIMATION_PLAYER_SHOT] = t3f_load_animation_from_bitmap("data/player_shot.png");
	if(!animation[ANIMATION_PLAYER_SHOT])
	{
		return false;
	}
	animation[ANIMATION_ENEMY_SHOT] = t3f_load_animation_from_bitmap("data/enemy_shot.png");
	if(!animation[ANIMATION_ENEMY_SHOT])
	{
		return false;
	}
	animation[ANIMATION_SPIRIT] = t3f_create_animation();
	if(!animation[ANIMATION_SPIRIT])
	{
		return false;
	}
	bp = al_load_bitmap("data/spirit0.png");
	t3f_animation_add_bitmap(animation[ANIMATION_SPIRIT], bp);
	t3f_animation_add_frame(animation[ANIMATION_SPIRIT], 0, 0, 0, 0, al_get_bitmap_width(bp), al_get_bitmap_height(bp), 0, 5);
	bp = al_load_bitmap("data/spirit1.png");
	t3f_animation_add_bitmap(animation[ANIMATION_SPIRIT], bp);
	t3f_animation_add_frame(animation[ANIMATION_SPIRIT], 1, 0, 0, 0, al_get_bitmap_width(bp), al_get_bitmap_height(bp), 0, 5);
	bp = al_load_bitmap("data/spirit2.png");
	t3f_animation_add_bitmap(animation[ANIMATION_SPIRIT], bp);
	t3f_animation_add_frame(animation[ANIMATION_SPIRIT], 2, 0, 0, 0, al_get_bitmap_width(bp), al_get_bitmap_height(bp), 0, 5);
	animation[ANIMATION_LOGO] = t3f_load_animation_from_bitmap("data/logo.png");
	if(!animation[ANIMATION_LOGO])
	{
		return false;
	}
	animation[ANIMATION_TITLE] = t3f_load_animation_from_bitmap("data/title_logo.png");
	if(!animation[ANIMATION_TITLE])
	{
		return false;
	}
	sprite_sheet = al_create_bitmap(512, 512);
	if(!sprite_sheet)
	{
		return false;
	}
	t3f_start_sprite_sheet(sprite_sheet, T3F_SPRITE_SHEET_TYPE_SPRITES);
	t3f_add_animation_to_sprite_sheet(animation[ANIMATION_PLAYER]);
	t3f_add_animation_to_sprite_sheet(animation[ANIMATION_DEMON]);
	t3f_add_animation_to_sprite_sheet(animation[ANIMATION_ARCHDEMON]);
	t3f_add_animation_to_sprite_sheet(animation[ANIMATION_PLAYER_SHOT]);
	t3f_add_animation_to_sprite_sheet(animation[ANIMATION_ENEMY_SHOT]);
	t3f_add_animation_to_sprite_sheet(animation[ANIMATION_SPIRIT]);
	t3f_finish_sprite_sheet();
	font[FONT_GAME] = al_load_bitmap_font("data/default_font.png");
	if(!font[FONT_GAME])
	{
		return false;
	}
	sample[SAMPLE_GAME_OVER] = al_load_sample("data/game_over.ogg");
	if(!sample[SAMPLE_GAME_OVER])
	{
		return false;
	}
	sample[SAMPLE_LEVEL_UP] = al_load_sample("data/level_up.ogg");
	if(!sample[SAMPLE_LEVEL_UP])
	{
		return false;
	}
	sample[SAMPLE_MAX_MULTIPLIER] = al_load_sample("data/max_multiplier.ogg");
	if(!sample[SAMPLE_MAX_MULTIPLIER])
	{
		return false;
	}
	sample[SAMPLE_HIGH_SCORE] = al_load_sample("data/high_score.ogg");
	if(!sample[SAMPLE_HIGH_SCORE])
	{
		return false;
	}
	sample[SAMPLE_TWIN_SHOT] = al_load_sample("data/twin_shot.ogg");
	if(!sample[SAMPLE_TWIN_SHOT])
	{
		return false;
	}
	sample[SAMPLE_TRIPLE_SHOT] = al_load_sample("data/triple_shot.ogg");
	if(!sample[SAMPLE_TRIPLE_SHOT])
	{
		return false;
	}
	sample[SAMPLE_POWERUP] = al_load_sample("data/powerup.ogg");
	if(!sample[SAMPLE_POWERUP])
	{
		return false;
	}
	sample[SAMPLE_SHOOT] = al_load_sample("data/shoot.ogg");
	if(!sample[SAMPLE_SHOOT])
	{
		return false;
	}
	sample[SAMPLE_HIT] = al_load_sample("data/hit.ogg");
	if(!sample[SAMPLE_HIT])
	{
		return false;
	}
	sample[SAMPLE_DIE] = al_load_sample("data/die.ogg");
	if(!sample[SAMPLE_DIE])
	{
		return false;
	}
	sample[SAMPLE_MULTIPLIER] = al_load_sample("data/multiplier.ogg");
	if(!sample[SAMPLE_MULTIPLIER])
	{
		return false;
	}
	cinema = load_cinema("data/intro.cin", 0);
	if(!cinema)
	{
		return false;
	}
	ending_cinema = load_cinema("data/ending.cin", 0);
	if(!ending_cinema)
	{
		return false;
	}
	level_color[0] = al_map_rgba(131, 0, 0, 255);
	level_color[1] = al_map_rgba(131, 22, 0, 255);
	level_color[2] = al_map_rgba(131, 46, 0, 255);
	level_color[3] = al_map_rgba(131, 68, 0, 255);
	level_color[4] = al_map_rgba(131, 99, 0, 255);
	level_color[5] = al_map_rgba(131, 126, 0, 255);
	level_color[6] = al_map_rgba(101, 131, 0, 255);
	level_color[7] = al_map_rgba(77, 131, 0, 255);
	level_color[8] = al_map_rgba(0, 131, 73, 255);
	level_color[9] = al_map_rgba(0, 105, 131, 255);
	level_color[10] = al_map_rgba(0, 135, 254, 255);
	hyperlink_set_driver(NULL);
	val = al_get_config_value(t3f_config, "Save Data", "High Score");
	if(val)
	{
		high_score = atoi(val);
	}
	t3f_play_music("data/title.xm", 0.0, 0.0);
	state = STATE_LOGO;
	return true;
}

int main(int argc, char * argv[])
{
	if(!initialize(argc, argv))
	{
		return -1;
	}
	t3f_run();
	return 0;
}
