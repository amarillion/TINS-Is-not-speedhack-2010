#include "t3f/t3f.h"
#include "t3f/hyperlink.h"
#include "main.h"
#include "title.h"
#include "enemy.h"

HYPERLINK_PAGE * menu[TITLE_MAX_MENUS] = {NULL};

int menu_proc_play(int i, void * p)
{
	current_menu = TITLE_MENU_DIFFICULTY;
	return 1;
}

int menu_proc_story(int i, void * p)
{
	cinema->position = 0;
	state = STATE_INTRO;
	return 1;
}

int menu_proc_quit(int i, void * p)
{
	t3f_exit();
	return 1;
}

int menu_proc_play_easy(int i, void * p)
{
	al_hide_mouse_cursor(t3f_display);
	enemy_spawn_logic = enemy_spawn_logic_easy;
	game_init();
	return 1;
}

int menu_proc_play_normal(int i, void * p)
{
	al_hide_mouse_cursor(t3f_display);
	enemy_spawn_logic = enemy_spawn_logic_normal;
	game_init();
	return 1;
}

int menu_proc_back(int i, void * p)
{
	current_menu = TITLE_MENU_MAIN;
	return 1;
}

bool title_init(void)
{
	menu[TITLE_MENU_MAIN] = hyperlink_page_create(0, 60);
	hyperlink_page_add_element_text(menu[TITLE_MENU_MAIN], menu_proc_play, "Play Game", font[FONT_GAME], 320, 240, al_map_rgba_f(1.0, 0.0, 0.0, 1.0), HYPERLINK_ELEMENT_FLAG_CENTER | HYPERLINK_ELEMENT_FLAG_CLICKABLE | HYPERLINK_ELEMENT_FLAG_SHADOW);
	hyperlink_page_add_element_text(menu[TITLE_MENU_MAIN], menu_proc_story, "View Story", font[FONT_GAME], 320, 260, al_map_rgba_f(1.0, 0.0, 0.0, 1.0), HYPERLINK_ELEMENT_FLAG_CENTER | HYPERLINK_ELEMENT_FLAG_CLICKABLE | HYPERLINK_ELEMENT_FLAG_SHADOW);
	hyperlink_page_add_element_text(menu[TITLE_MENU_MAIN], menu_proc_quit, "Exit to OS", font[FONT_GAME], 320, 280, al_map_rgba_f(1.0, 0.0, 0.0, 1.0), HYPERLINK_ELEMENT_FLAG_CENTER | HYPERLINK_ELEMENT_FLAG_CLICKABLE | HYPERLINK_ELEMENT_FLAG_SHADOW);
	menu[TITLE_MENU_DIFFICULTY] = hyperlink_page_create(0, 60);
	hyperlink_page_add_element_text(menu[TITLE_MENU_DIFFICULTY], menu_proc_play_easy, "Easy", font[FONT_GAME], 320, 240, al_map_rgba_f(1.0, 0.0, 0.0, 1.0), HYPERLINK_ELEMENT_FLAG_CENTER | HYPERLINK_ELEMENT_FLAG_CLICKABLE | HYPERLINK_ELEMENT_FLAG_SHADOW);
	hyperlink_page_add_element_text(menu[TITLE_MENU_DIFFICULTY], menu_proc_play_normal, "Normal", font[FONT_GAME], 320, 260, al_map_rgba_f(1.0, 0.0, 0.0, 1.0), HYPERLINK_ELEMENT_FLAG_CENTER | HYPERLINK_ELEMENT_FLAG_CLICKABLE | HYPERLINK_ELEMENT_FLAG_SHADOW);
	hyperlink_page_add_element_text(menu[TITLE_MENU_DIFFICULTY], menu_proc_back, "Back", font[FONT_GAME], 320, 280, al_map_rgba_f(1.0, 0.0, 0.0, 1.0), HYPERLINK_ELEMENT_FLAG_CENTER | HYPERLINK_ELEMENT_FLAG_CLICKABLE | HYPERLINK_ELEMENT_FLAG_SHADOW);
	state = STATE_TITLE;
	return true;
}

void title_logic(void)
{
	hyperlink_page_logic(menu[current_menu]);
}

void title_render(void)
{
	al_clear_to_color(al_map_rgba_f(0.0, 0.0, 0.0, 1.0));
	al_draw_textf(font[FONT_GAME], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), 320, 0, ALLEGRO_ALIGN_CENTRE, "High Score: %06d", high_score);
	t3f_draw_animation(animation[ANIMATION_TITLE], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), state_ticks, 320 - al_get_bitmap_width(animation[ANIMATION_TITLE]->bitmap[0]) / 2, 60, 0, 0);
	hyperlink_page_render(menu[current_menu]);
}
