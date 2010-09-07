#include "background.h"
#include <assert.h>
#include "color.h"
#include <stdio.h>
#include "game.h"
#include <math.h>

static const int TILEW = 128;
static const int TILEH = 128;

Background::Background(Engine &e) : Layer (e)
{
	xofst = 0;
	yofst = 0;
	texture = NULL;
}

void Background::init()
{
	texture = create_bitmap (TILEW, TILEH);
	assert (texture);

	int c1 = WHITE;
	int c2 = GREY;
	clear_to_color (texture, c1);
	circlefill(texture, TILEW / 2, TILEW / 2, 60, c2);
	circlefill (texture, TILEW / 2, TILEH / 2, 30, c1);
	
	drawing_mode (DRAW_MODE_TRANS, NULL, 0, 0);
	set_trans_blender (0, 0, 0, 160);
	rectfill (texture, 0, 0, TILEW, TILEH, BROWN);
	solid_mode();
}

Background::~ Background()
{
	if (texture) destroy_bitmap (texture);
	texture = NULL;
}

void Background::update()
{
	xofst++;
	yofst++;
}

void Background::draw(BITMAP * buffer)
{
	assert (texture);

	for (int x = (xofst % TILEW) - TILEW; x < buffer->w + TILEW; x += TILEW)
	{
		for (int y = (yofst % TILEH) - TILEH; y < buffer->h + TILEH; y += TILEH)
		{
			draw_sprite (buffer, texture, x, y);
		}
	}

}
