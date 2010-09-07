#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include "gui.h"

static ALLEGRO_FONT * t3f_internal_gui_font = NULL;
static ALLEGRO_COLOR t3f_internal_gui_menu_bg_color;
static ALLEGRO_COLOR t3f_internal_gui_menu_text_color;
static ALLEGRO_COLOR t3f_internal_gui_menu_selected_bg_color;
static ALLEGRO_COLOR t3f_internal_gui_menu_selected_text_color;

void t3f_select_menu_font(ALLEGRO_FONT * fp)
{
	t3f_internal_gui_font = fp;
}

void t3f_set_menu_colors(ALLEGRO_COLOR bg, ALLEGRO_COLOR text, ALLEGRO_COLOR selected_bg, ALLEGRO_COLOR selected_text)
{
	t3f_internal_gui_menu_bg_color = bg;
	t3f_internal_gui_menu_text_color = text;
	t3f_internal_gui_menu_selected_bg_color = selected_bg;
	t3f_internal_gui_menu_selected_text_color = selected_text;
}

T3F_MENU_PLAYER * t3f_start_menu(T3F_MENU * mp, int flags)
{
	T3F_MENU_PLAYER * player = NULL;
	
	player = malloc(sizeof(T3F_MENU_PLAYER));
	if(!player)
	{
		return NULL;
	}
	player->menu = mp;
	player->flags = flags;
	
	return player;
}

int t3f_update_menu(T3F_MENU_PLAYER * mp)
{
	return 0;
}

static int t3f_get_menu_width(T3F_MENU * mp, int flags)
{
	int tw = 0;
	int maxw = 0;
	int i;
	
	if(flags & T3F_GUI_MENU_FLAG_CONTEXT)
	{
		for(i = 0; mp[i].text != NULL; i++)
		{
			tw = al_get_text_width(t3f_internal_gui_font, mp[i].text);
			if(tw > maxw)
			{
				maxw = tw;
			}
		}
		return maxw + al_get_text_width(t3f_internal_gui_font, "  ") + 4;
	}
	else
	{
		return al_get_display_width();
	}
	return 0;
}

static int t3f_get_menu_height(T3F_MENU * mp, int flags)
{
	int i;
	int height = 0;
	
	if(flags & T3F_GUI_MENU_FLAG_CONTEXT)
	{
		for(i = 0; mp[i].text != NULL; i++)
		{
			height += al_get_font_line_height(t3f_internal_gui_font);
		}
		return height + 4;
	}
	else
	{
		return al_get_font_line_height(t3f_internal_gui_font) + 4;
	}
	return 0;
}

static void t3f_render_menu_item(T3F_MENU * mp, float x, float y, int w, int h, bool selected)
{
	if(selected)
	{
		al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA, al_map_rgba_f(1.0, 1.0, 1.0, 1.0));
		al_draw_filled_rectangle(x + 1.5, y + 1.5, x + w - 1.5, y + h - 1.5, t3f_internal_gui_menu_selected_bg_color);
		al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA, t3f_internal_gui_menu_selected_text_color);
		al_draw_textf(t3f_internal_gui_font, x + 1, y + 1, 0, " %s", mp->text);
	}
	else
	{
		al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA, al_map_rgba_f(1.0, 1.0, 1.0, 1.0));
		al_draw_filled_rectangle(x + 1.5, y + 1.5, x + w - 1.5, y + h - 1.5, t3f_internal_gui_menu_bg_color);
		al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA, t3f_internal_gui_menu_text_color);
		al_draw_textf(t3f_internal_gui_font, x + 1, y + 1, 0, " %s", mp->text);
	}
}

static void t3f_render_whole_menu(T3F_MENU * mp, float x, float y, int flags)
{
	float current_x, current_y;
	float gx, gy;
	int i;
	int mw = t3f_get_menu_width(mp, flags);
	int mh = t3f_get_menu_height(mp, flags);
	
	current_x = x;
	current_y = y;
	gx = current_x + 0.5;
	gy = current_y + 0.5;
	al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA, al_map_rgba_f(1.0, 1.0, 1.0, 1.0));
	al_draw_filled_rectangle(gx, gy, gx + mw, gy + mh, t3f_internal_gui_menu_bg_color);
	al_draw_line(gx + 1, gy, gx + mw - 1, gy, al_map_rgba_f(1.0, 1.0, 1.0, 0.5), 1.0);
	al_draw_line(gx, gy, gx, gy + mh - 1, al_map_rgba_f(1.0, 1.0, 1.0, 0.5), 1.0);
	al_draw_line(gx + mw - 1, gy, gx + mw - 1, gy + mh - 1, al_map_rgba_f(0.0, 0.0, 0.0, 0.5), 1.0);
	al_draw_line(gx, gy + mh - 1, gx + mw, gy + mh - 1, al_map_rgba_f(0.0, 0.0, 0.0, 0.5), 1.0);
	for(i = 0; mp[i].text != NULL; i++)
	{
		t3f_render_menu_item(&mp[i], current_x, current_y, mw, al_get_font_line_height(t3f_internal_gui_font), false);
		if(flags & T3F_GUI_MENU_FLAG_CONTEXT)
		{
			current_y += al_get_font_line_height(t3f_internal_gui_font);
		}
		else
		{
			current_x += al_get_text_width(t3f_internal_gui_font, mp[i].text) + al_get_text_width(t3f_internal_gui_font, "  ");
		}
	}
}

static void t3f_render_menu_recursive(T3F_MENU * mp, float x, float y, int flags)
{
	float current_x, current_y;
	int i;
	int w = t3f_get_menu_width(mp, flags);
	
	current_x = x;
	current_y = y;
	t3f_render_whole_menu(mp, current_x, current_y, flags);
	for(i = 0; mp[i].text != NULL; i++)
	{
		if(mp[i].flags & T3F_GUI_MENU_FLAG_OPEN)
		{
			if(flags & T3F_GUI_MENU_FLAG_CONTEXT)
			{
				t3f_render_menu_recursive((T3F_MENU *)(mp[i].child), current_x + w, current_y, T3F_GUI_MENU_FLAG_CONTEXT);
			}
			else
			{
				t3f_render_menu_recursive((T3F_MENU *)(mp[i].child), current_x, current_y + al_get_font_line_height(t3f_internal_gui_font) + 4, T3F_GUI_MENU_FLAG_CONTEXT);
			}
		}
		if(flags & T3F_GUI_MENU_FLAG_CONTEXT)
		{
			current_y += al_get_font_line_height(t3f_internal_gui_font);
		}
		else
		{
			current_x += al_get_text_width(t3f_internal_gui_font, mp[i].text) + al_get_text_width(t3f_internal_gui_font, "  ");
		}
	}
}

void t3f_render_menu(T3F_MENU_PLAYER * mp)
{
	float current_x, current_y;
	ALLEGRO_STATE old_state;
	
	current_x = mp->x;
	current_y = mp->y;
	al_store_state(&old_state, ALLEGRO_STATE_BLENDER);
	t3f_render_menu_recursive(mp->menu, mp->x, mp->y, mp->flags);
	al_restore_state(&old_state);
}

int t3f_close_menu(T3F_MENU_PLAYER * mp)
{
	int ret = mp->mouse_sel;
	
	free(mp);
	
	return ret;
}

T3F_DIALOG_PLAYER * t3f_start_gui(T3F_DIALOG * dialog, int focus_obj)
{
	T3F_DIALOG_PLAYER * player = NULL;
	
	player = malloc(sizeof(T3F_DIALOG_PLAYER));
	if(!player)
	{
		return NULL;
	}
	
	return player;
}

int t3f_update_gui(T3F_DIALOG_PLAYER * dp)
{
	int i, ret = 0;
	
	for(i = 0; dp->dialog[i].proc != NULL; i++)
	{
		ret = dp->dialog[i].proc(T3F_GUI_MESSAGE_LOGIC, (struct T3F_DIALOG *)(&dp->dialog[i]), dp->key);
	}
	
	return ret;
}

void t3f_render_gui(T3F_DIALOG_PLAYER * dp)
{
}

int t3f_close_gui(T3F_DIALOG_PLAYER * dp)
{
	int ret = dp->focus_obj;
	
	free(dp);
	
	return ret;
}
