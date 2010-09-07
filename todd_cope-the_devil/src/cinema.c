#include "t3f/t3f.h"
#include "t3f/3d.h"
#include "cinema.h"

CINEMA * create_cinema(void)
{
	CINEMA * cp;
	
	cp = malloc(sizeof(CINEMA));
	if(!cp)
	{
		return NULL;
	}
	cp->animations = 0;
	cp->fonts = 0;
	cp->frames = 0;
	return cp;
}

void destroy_cinema(CINEMA * cp)
{
	int i;
	
	for(i = 0; i < cp->animations; i++)
	{
		t3f_destroy_animation(cp->animation[i]);
	}
	for(i = 0; i < cp->fonts; i++)
	{
		al_destroy_font(cp->font[i]);
	}
	free(cp);
}

void add_cinema_animation(CINEMA * cp, T3F_ANIMATION * ap)
{
	if(cp && ap && cp->animations < CINEMA_MAX_ANIMATIONS)
	{
		cp->animation[cp->animations] = ap;
		cp->animations++;
	}
}

void delete_cinema_animation(CINEMA * cp, int index)
{
	int i;
	
	if(cp && index < cp->animations)
	{
		t3f_destroy_animation(cp->animation[index]);
		for(i = index; i < cp->animations - 1; i++)
		{
			cp->animation[i] = cp->animation[i + 1];
		}
		cp->animations--;
	}
}

void add_cinema_font(CINEMA * cp, ALLEGRO_BITMAP * bp)
{
	int ranges[] = {32, 126};
	
	if(cp && bp && cp->fonts < CINEMA_MAX_FONTS)
	{
		cp->font_bitmap[cp->fonts] = bp;
		cp->font[cp->fonts] = al_grab_font_from_bitmap(bp, 1, ranges);
		cp->fonts++;
	}
}

void delete_cinema_font(CINEMA * cp, int index)
{
	int i;
	
	if(cp && index < cp->fonts)
	{
		al_destroy_font(cp->font[index]);
		for(i = index; i < cp->fonts - 1; i++)
		{
			cp->font[i] = cp->font[i + 1];
		}
		cp->fonts--;
	}
}

CINEMA * load_cinema(const char * fn, int flags)
{
	CINEMA * cp;
	char header[16] = {0};
	int ranges[] = {32, 126};
	ALLEGRO_FILE * fp;
	int i, j;
	
	fp = al_fopen(fn, "rb");
	if(!fp)
	{
		return NULL;
	}
	if(al_fread(fp, header, 16) != 16)
	{
		al_fclose(fp);
		return NULL;
	}
	if(memcmp(header, "CINMA", 5))
	{
		al_fclose(fp);
		return NULL;
	}
	cp = create_cinema();
	if(!cp)
	{
		al_fclose(fp);
		return NULL;
	}
	cp->animations = al_fread16le(fp);
	for(i = 0; i < cp->animations; i++)
	{
		cp->animation[i] = t3f_load_animation_entry(fp);
	}
	cp->fonts = al_fread16le(fp);
	for(i = 0; i < cp->fonts; i++)
	{
		cp->font_bitmap[i] = al_load_bitmap_f(fp, ".png");
		cp->font[i] = al_grab_font_from_bitmap(cp->font_bitmap[i], 1, ranges);
		if(!(flags & CINEMA_LOAD_FLAG_EDITOR))
		{
			al_destroy_bitmap(cp->font_bitmap[i]);
		}
	}
	cp->frames = al_fread16le(fp);
	for(i = 0; i < cp->frames; i++)
	{
		cp->frame[i].entities = al_fread16le(fp);
		for(j = 0; j < cp->frame[i].entities; j++)
		{
			cp->frame[i].entity[j].type = al_fgetc(fp);
			cp->frame[i].entity[j].index = al_fread16le(fp);
			cp->frame[i].entity[j].x = t3f_fread_float(fp);
			cp->frame[i].entity[j].y = t3f_fread_float(fp);
			cp->frame[i].entity[j].z = t3f_fread_float(fp);
			cp->frame[i].entity[j].scale = t3f_fread_float(fp);
			cp->frame[i].entity[j].angle = t3f_fread_float(fp);
			memset(cp->frame[i].entity[j].data, 256, 1);
			al_fread(fp, cp->frame[i].entity[j].data, al_fread16le(fp));
		}
		cp->frame[i].ticks = al_fread16le(fp);
	}
	al_fclose(fp);
	return cp;
}

bool save_cinema(CINEMA * cp, const char * fn)
{
	char header[16] = {'C', 'I', 'N', 'M', 'A'};
	ALLEGRO_FILE * fp;
	int i, j;
	
	fp = al_fopen(fn, "wb");
	if(!fp)
	{
		return false;
	}
	if(al_fwrite(fp, header, 16) != 16)
	{
		al_fclose(fp);
		return false;
	}
	al_fwrite16le(fp, cp->animations);
	for(i = 0; i < cp->animations; i++)
	{
		t3f_save_animation_entry(cp->animation[i], fp);
	}
	al_fwrite16le(fp, cp->fonts);
	for(i = 0; i < cp->fonts; i++)
	{
		al_save_bitmap_f(fp, ".png", cp->font_bitmap[i]);
	}
	al_fwrite16le(fp, cp->frames);
	for(i = 0; i < cp->frames; i++)
	{
		al_fwrite16le(fp, cp->frame[i].entities);
		for(j = 0; j < cp->frame[i].entities; j++)
		{
			al_fputc(fp, cp->frame[i].entity[j].type);
			al_fwrite16le(fp, cp->frame[i].entity[j].index);
			t3f_fwrite_float(fp, cp->frame[i].entity[j].x);
			t3f_fwrite_float(fp, cp->frame[i].entity[j].y);
			t3f_fwrite_float(fp, cp->frame[i].entity[j].z);
			t3f_fwrite_float(fp, cp->frame[i].entity[j].scale);
			t3f_fwrite_float(fp, cp->frame[i].entity[j].angle);
			al_fwrite16le(fp, strlen(cp->frame[i].entity[j].data));
			al_fwrite(fp, cp->frame[i].entity[j].data, strlen(cp->frame[i].entity[j].data));
		}
		al_fwrite16le(fp, cp->frame[i].ticks);
	}
	al_fclose(fp);
	return true;
}

void cinema_logic(CINEMA * cp)
{
	
	/* skip cinema */
	if(t3f_key[ALLEGRO_KEY_ESCAPE])
	{
		cp->position = cp->frames;
		t3f_key[ALLEGRO_KEY_ESCAPE] = 0;
	}
	cp->tick++;
	if(t3f_key[ALLEGRO_KEY_SPACE] || t3f_key[ALLEGRO_KEY_ENTER] || cp->tick > 240)
	{
		cp->position++;
		t3f_key[ALLEGRO_KEY_SPACE] = 0;
		t3f_key[ALLEGRO_KEY_ENTER] = 0;
		cp->tick = 0;
	}
	
}

void cinema_render(CINEMA * cp)
{
	int i;
	
	al_clear_to_color(al_map_rgba_f(0.0, 0.0, 0.0, 1.0));
	if(cp->position >= cp->frames)
	{
		return;
	}
	for(i = 0; i < cp->frame[cp->position].entities; i++)
	{
		switch(cp->frame[cp->position].entity[i].type)
		{
			case CINEMA_ENTITY_TEXT:
			{
				al_draw_text(cp->font[cp->frame[cp->position].entity[i].index], al_map_rgba_f(0.0, 0.0, 0.0, 0.8), cp->frame[cp->position].entity[i].x + 2, cp->frame[cp->position].entity[i].y + 2, 0, cp->frame[cp->position].entity[i].data);
				al_draw_text(cp->font[cp->frame[cp->position].entity[i].index], al_map_rgba_f(1.0, 0.0, 0.0, 1.0), cp->frame[cp->position].entity[i].x, cp->frame[cp->position].entity[i].y, 0, cp->frame[cp->position].entity[i].data);
				break;
			}
			case CINEMA_ENTITY_ANIMATION:
			{
				t3f_draw_animation(cp->animation[cp->frame[cp->position].entity[i].index], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), 0, cp->frame[cp->position].entity[i].x, cp->frame[cp->position].entity[i].y, cp->frame[cp->position].entity[i].z, 0);
				break;
			}
		}
	}
}
