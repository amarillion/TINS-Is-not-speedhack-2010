#ifndef T3F_GUI_H
#define T3F_GUI_H

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>

/* GUI messages */
#define T3F_GUI_MESSAGE_LOGIC      0
#define T3F_GUI_MESSAGE_RENDER     1

/* menu flags */
#define T3F_GUI_MENU_FLAG_CONTEXT  1
#define T3F_GUI_MENU_FLAG_OPEN     2

#define T3F_GUI_MAX_MENU_CHILDREN 64

//struct T3F_MENU;
//struct T3F_DIALOG;

typedef struct T3F_MENU
{
	
	char * text;
	int (*proc)(void);
	struct T3F_MENU * child;
	void * dp;
	int flags;
	
} T3F_MENU;

typedef struct T3F_DIALOG
{
	
	T3F_MENU *menu;                      /* the menu itself */
	int bar;                         /* set if it is a top level menu bar */
	int size;                        /* number of items in the menu */
	int sel[T3F_GUI_MAX_MENU_CHILDREN]; /* selected item */
	int x, y, w, h;                  /* screen position of the menu */
	int (*proc)(void);               /* callback function */
	int flags;

	int mouse_button_was_pressed;    /* set if mouse button pressed on last iteration */
	int back_from_child;             /* set if a child was activated on last iteration */    
	int timestamp;                   /* timestamp for gui_timer events */
	int mouse_sel;                   /* item the mouse is currently over */
	int redraw;                      /* set if redrawing is required */
	int auto_open;                   /* set if menu auto-opening is activated */
	int ret;                         /* return value */

	struct MENU_PLAYER *parent;      /* the parent menu, or NULL for root */
	struct MENU_PLAYER *child;       /* the child menu, or NULL for none */
	
} T3F_MENU_PLAYER;
   
typedef struct
{
	
	int (*proc)(int,struct T3F_DIALOG *, int);
	int x, y, w, h;
	int fg, bg;
	int key;
	int flags;
	int d1, d2;
	void *dp, *dp2, *dp3;
	
} T3F_DIALOG;

typedef struct
{
	
	int obj;
	int res;
	int mouse_obj;
	int focus_obj;
	int click_wait;
	int mouse_ox, mouse_oy;
	int mouse_oz;
	int mouse_b;
	int key;
	T3F_DIALOG * dialog;
	
} T3F_DIALOG_PLAYER;

void t3f_select_menu_font(ALLEGRO_FONT * fp);
void t3f_set_menu_colors(ALLEGRO_COLOR bg, ALLEGRO_COLOR text, ALLEGRO_COLOR selected_bg, ALLEGRO_COLOR selected_text);
T3F_MENU_PLAYER * t3f_start_menu(T3F_MENU * mp, int flags);
int t3f_update_menu(T3F_MENU_PLAYER * mp);
void t3f_render_menu(T3F_MENU_PLAYER * mp);
int t3f_close_menu(T3F_MENU_PLAYER * mp);

T3F_DIALOG_PLAYER * t3f_start_gui(T3F_DIALOG *dialog, int focus_obj);
int t3f_update_gui(T3F_DIALOG_PLAYER * dp);
void t3f_render_gui(T3F_DIALOG_PLAYER * dp);
int t3f_close_gui(T3F_DIALOG_PLAYER * dp);

#endif
