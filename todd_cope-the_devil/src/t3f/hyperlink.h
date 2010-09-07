#ifndef T3F_HYPERLINK_H
#define T3F_HYPERLINK_H

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>

#define HYPERLINK_PAGE_MAX_ELEMENTS 128

#define HYPERLINK_ELEMENT_TYPE_TEXT      0
#define HYPERLINK_ELEMENT_TYPE_IMAGE     1

#define HYPERLINK_ELEMENT_FLAG_CLICKABLE 1
#define HYPERLINK_ELEMENT_FLAG_CENTER    2
#define HYPERLINK_ELEMENT_FLAG_SHADOW    4
#define HYPERLINK_ELEMENT_FLAG_BLIT      8
#define HYPERLINK_ELEMENT_FLAG_AUTOHIDE 16
#define HYPERLINK_ELEMENT_FLAG_COPY     32
#define HYPERLINK_ELEMENT_FLAG_NO_HOVER 64

#define HYPERLINK_PAGE_FLAG_DISABLED     1

typedef struct
{
	
	int type;
	void * data;
	void * aux_data;
	ALLEGRO_COLOR color;
	int flags;
	int (*proc)(int, void *);
	
	int ox, oy;
	int d1, d2, d3;
	
} HYPERLINK_ELEMENT;

typedef struct
{
	
	HYPERLINK_ELEMENT element[HYPERLINK_PAGE_MAX_ELEMENTS];
	int elements;
	int flags;
	
	int ox, oy;
	
	int hover_element;
	
} HYPERLINK_PAGE;

typedef struct
{
	
	float(*get_element_width)(HYPERLINK_ELEMENT * ep);
	float(*get_element_height)(HYPERLINK_ELEMENT * ep);
	void(*render_element)(HYPERLINK_PAGE * pp, int i, bool hover);
	
} HYPERLINK_DRIVER;

void hyperlink_set_driver(HYPERLINK_DRIVER * dp);
HYPERLINK_PAGE * hyperlink_page_create(int ox, int oy);
void hyperlink_page_destroy(HYPERLINK_PAGE * pp);

int hyperlink_page_add_element_image(HYPERLINK_PAGE * pp, int (*proc)(int, void *), void * bp, int ox, int oy, int flags);
int hyperlink_page_add_element_text(HYPERLINK_PAGE * pp, int (*proc)(int, void *), char * text, void * fp, int ox, int oy, ALLEGRO_COLOR color, int flags);
void hyperlink_page_centre(HYPERLINK_PAGE * pp, float oy, float my);

void hyperlink_page_logic(HYPERLINK_PAGE * pp);
void hyperlink_page_render(HYPERLINK_PAGE * pp);

#endif
