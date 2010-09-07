#ifndef CINEMA_H
#define CINEMA_H

#include "t3f/animation.h"

#define CINEMA_MAX_ANIMATIONS    256
#define CINEMA_MAX_FONTS           8

#define CINEMA_ENTITY_TEXT         0
#define CINEMA_ENTITY_ANIMATION    1
#define CINEMA_ENTITY_MAX_DATA   256

#define CINEMA_FRAME_MAX_ENTITIES 32
#define CINEMA_MAX_FRAMES         64

#define CINEMA_LOAD_FLAG_EDITOR    1

typedef struct
{
	
	int type;
	int index;
	float x, y, z, scale, angle;
	char data[CINEMA_ENTITY_MAX_DATA];
	
} CINEMA_ENTITY;

typedef struct
{
	
	CINEMA_ENTITY entity[CINEMA_FRAME_MAX_ENTITIES];
	int entities;
	int ticks;
	
} CINEMA_FRAME;

typedef struct
{
	
	T3F_ANIMATION * animation[CINEMA_MAX_ANIMATIONS];
	int animations;
	
	ALLEGRO_BITMAP * font_bitmap[CINEMA_MAX_FONTS];
	ALLEGRO_FONT * font[CINEMA_MAX_FONTS];
	int fonts;
	
	CINEMA_FRAME frame[CINEMA_MAX_FRAMES];
	int frames;
	
	int position;
	int tick;
	
} CINEMA;

CINEMA * create_cinema(void);
void destroy_cinema(CINEMA * cp);
void add_cinema_animation(CINEMA * cp, T3F_ANIMATION * ap);
void delete_cinema_animation(CINEMA * cp, int index);
void add_cinema_font(CINEMA * cp, ALLEGRO_BITMAP * bp);
void delete_cinema_font(CINEMA * cp, int index);

CINEMA * load_cinema(const char * fn, int flags);
bool save_cinema(CINEMA * cp, const char * fn);

void cinema_logic(CINEMA * cp);
void cinema_render(CINEMA * cp);

#endif
