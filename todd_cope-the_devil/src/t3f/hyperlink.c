#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include "t3f.h"
#include "hyperlink.h"

static bool t3f_hyperlink_left_clicked = 0;
HYPERLINK_DRIVER hyperlink_allegro_driver;
static HYPERLINK_DRIVER * hyperlink_current_driver = NULL;
static bool hyperlink_check_hover_y(HYPERLINK_PAGE * pp, int i);

static float allegro_get_element_width(HYPERLINK_ELEMENT * ep)
{
	switch(ep->type)
	{
		case HYPERLINK_ELEMENT_TYPE_TEXT:
		{
			return al_get_text_width((ALLEGRO_FONT *)ep->aux_data, ep->data);
		}
		case HYPERLINK_ELEMENT_TYPE_IMAGE:
		{
			return al_get_bitmap_width(((ALLEGRO_BITMAP *)(ep->data)));
		}
	}
	return 0.0;
}

static float allegro_get_element_height(HYPERLINK_ELEMENT * ep)
{
	switch(ep->type)
	{
		case HYPERLINK_ELEMENT_TYPE_TEXT:
		{
			return al_get_font_line_height((ALLEGRO_FONT *)ep->aux_data);
		}
		case HYPERLINK_ELEMENT_TYPE_IMAGE:
		{
			return al_get_bitmap_height(((ALLEGRO_BITMAP *)(ep->data)));
		}
	}
	return 0.0;
}

static void allegro_render_element(HYPERLINK_PAGE * pp, int i, bool hover)
{
	int sx, sy;
	
	if(hover)
	{
		sx = -4;
		sy = -4;
	}
	else
	{
		sx = -2;
		sy = -2;
	}
	
	switch(pp->element[i].type)
	{
		case HYPERLINK_ELEMENT_TYPE_TEXT:
		{
			if(pp->element[i].flags & HYPERLINK_ELEMENT_FLAG_SHADOW)
			{
				if(!(pp->element[i].flags & HYPERLINK_ELEMENT_FLAG_AUTOHIDE) || hyperlink_check_hover_y(pp, i))
				{
					if(pp->element[i].flags & HYPERLINK_ELEMENT_FLAG_CENTER)
					{
						al_draw_text((ALLEGRO_FONT *)pp->element[i].aux_data, al_map_rgba_f(0.0, 0.0, 0.0, 0.5), pp->ox + pp->element[i].ox, pp->oy + pp->element[i].oy, ALLEGRO_ALIGN_CENTRE, (char *)pp->element[i].data);
						al_draw_text((ALLEGRO_FONT *)pp->element[i].aux_data, pp->element[i].color, pp->ox + pp->element[i].ox + sx, pp->oy + pp->element[i].oy + sy, ALLEGRO_ALIGN_CENTRE, (char *)pp->element[i].data);
					}
					else
					{
						al_draw_text((ALLEGRO_FONT *)pp->element[i].aux_data, al_map_rgba_f(0.0, 0.0, 0.0, 0.5), pp->ox + pp->element[i].ox, pp->oy + pp->element[i].oy, 0, (char *)pp->element[i].data);
						al_draw_text((ALLEGRO_FONT *)pp->element[i].aux_data, pp->element[i].color, pp->ox + pp->element[i].ox + sx, pp->oy + pp->element[i].oy + sy, 0, (char *)pp->element[i].data);
					}
				}
			}
			else
			{
			}
			break;
		}
		case HYPERLINK_ELEMENT_TYPE_IMAGE:
		{
			if(pp->element[i].flags & HYPERLINK_ELEMENT_FLAG_SHADOW)
			{
				if(pp->element[i].flags & HYPERLINK_ELEMENT_FLAG_CENTER)
				{
					if(pp->element[i].data)
					{
						al_draw_tinted_bitmap((ALLEGRO_BITMAP *)(pp->element[i].data), al_map_rgba_f(0.0, 0.0, 0.0, 0.5), pp->ox + pp->element[i].ox - al_get_bitmap_width(((ALLEGRO_BITMAP *)(pp->element[i].data))) / 2, pp->oy + pp->element[i].oy - al_get_bitmap_width(((ALLEGRO_BITMAP *)(pp->element[i].data))) / 2, 0);
						al_draw_bitmap((ALLEGRO_BITMAP *)(pp->element[i].data), pp->ox + pp->element[i].ox - al_get_bitmap_width(((ALLEGRO_BITMAP *)(pp->element[i].data))) / 2 - sx, pp->oy + pp->element[i].oy - al_get_bitmap_width(((ALLEGRO_BITMAP *)(pp->element[i].data))) / 2 - sy, 0);
					}
				}
				else
				{
					if(pp->element[i].data)
					{
						al_draw_tinted_bitmap((ALLEGRO_BITMAP *)(pp->element[i].data), al_map_rgba_f(0.0, 0.0, 0.0, 0.5), pp->ox + pp->element[i].ox, pp->oy + pp->element[i].oy, 0);
						al_draw_bitmap((ALLEGRO_BITMAP *)(pp->element[i].data), pp->ox + pp->element[i].ox - sx, pp->oy + pp->element[i].oy - sy, 0);
					}
				}
			}
			break;
		}
		default:
		{
			break;
		}
	}
}

void hyperlink_set_driver(HYPERLINK_DRIVER * dp)
{
	if(dp == NULL)
	{
		hyperlink_allegro_driver.get_element_width = allegro_get_element_width;
		hyperlink_allegro_driver.get_element_height = allegro_get_element_height;
		hyperlink_allegro_driver.render_element = allegro_render_element;
		hyperlink_current_driver = &hyperlink_allegro_driver;
	}
	else
	{
		hyperlink_current_driver = dp;
	}
}

HYPERLINK_PAGE * hyperlink_page_create(int ox, int oy)
{
	HYPERLINK_PAGE * lp;
	lp = malloc(sizeof(HYPERLINK_PAGE));
	if(!lp)
	{
		return NULL;
	}
	lp->elements = 0;
	lp->ox = ox;
	lp->oy = oy;
	lp->hover_element = -1;
	return lp;
}

void hyperlink_page_destroy(HYPERLINK_PAGE * pp)
{
	int i;
	
	for(i = 0; i < pp->elements; i++)
	{
		if(pp->element[i].type == HYPERLINK_ELEMENT_TYPE_TEXT && pp->element[i].flags & HYPERLINK_ELEMENT_FLAG_COPY)
		{
			free(pp->element[i].data);
		}
	}
	free(pp);
}

int hyperlink_page_add_element_image(HYPERLINK_PAGE * pp, int (*proc)(int, void *), void * bp, int ox, int oy, int flags)
{
	pp->element[pp->elements].type = HYPERLINK_ELEMENT_TYPE_IMAGE;
	pp->element[pp->elements].proc = proc;
	pp->element[pp->elements].data = bp;
	pp->element[pp->elements].ox = ox;
	pp->element[pp->elements].oy = oy;
	pp->element[pp->elements].flags = flags;
	pp->elements++;
	return 1;
}

int hyperlink_page_add_element_text(HYPERLINK_PAGE * pp, int (*proc)(int, void *), char * text, void * fp, int ox, int oy, ALLEGRO_COLOR color, int flags)
{
	pp->element[pp->elements].type = HYPERLINK_ELEMENT_TYPE_TEXT;
	pp->element[pp->elements].proc = proc;
	if(flags & HYPERLINK_ELEMENT_FLAG_COPY)
	{
		pp->element[pp->elements].data = malloc(strlen(text) + 1);
		memcpy(pp->element[pp->elements].data, text, strlen(text) + 1);
	}
	else
	{
		pp->element[pp->elements].data = text;
	}
	pp->element[pp->elements].aux_data = fp;
	pp->element[pp->elements].ox = ox;
	pp->element[pp->elements].oy = oy;
	pp->element[pp->elements].color = color;
	pp->element[pp->elements].flags = flags;
	pp->elements++;
	return 1;
}

void hyperlink_page_centre(HYPERLINK_PAGE * pp, float oy, float my)
{
	int i;
	float top = 1000.0;
	float bottom = 0.0;
	int top_i;
	int bottom_i;
	float dheight = my - oy;
	float height;
	float offset;
	
	for(i = 0; i < pp->elements; i++)
	{
		if(pp->element[i].oy < top)
		{
			top = pp->element[i].oy;
			top_i = i;
		}
		if(pp->element[i].oy + al_get_font_line_height((ALLEGRO_FONT *)pp->element[i].aux_data) > bottom)
		{
			bottom = pp->element[i].oy + al_get_font_line_height((ALLEGRO_FONT *)pp->element[i].aux_data);
			bottom_i = i;
		}
	}
	height = bottom - top;
	offset = oy + dheight / 2.0 - height / 2.0;
	pp->oy = offset - top;
}

static bool hyperlink_check_hover_x(HYPERLINK_PAGE * pp, int i)
{
	if(pp->element[i].flags & HYPERLINK_ELEMENT_FLAG_CENTER)
	{
		if(t3f_mouse_x >= pp->ox + pp->element[i].ox - hyperlink_current_driver->get_element_width(&pp->element[i]) / 2 && t3f_mouse_x < pp->ox + pp->element[i].ox + hyperlink_current_driver->get_element_width(&pp->element[i]) / 2)
		{
			return true;
		}
	}
	else
	{
		if(t3f_mouse_x >= pp->ox + pp->element[i].ox && t3f_mouse_x < pp->ox + pp->element[i].ox + hyperlink_current_driver->get_element_width(&pp->element[i]))
		{
			return true;
		}
	}
	return false;
}

static bool hyperlink_check_hover_y(HYPERLINK_PAGE * pp, int i)
{
	if(t3f_mouse_y >= pp->oy + pp->element[i].oy && t3f_mouse_y < pp->oy + pp->element[i].oy + hyperlink_current_driver->get_element_height(&pp->element[i]))
	{
		return true;
	}
	return false;
}

static bool hyperlink_check_hover(HYPERLINK_PAGE * pp, int i)
{
	return hyperlink_check_hover_x(pp, i) && hyperlink_check_hover_y(pp, i) && !(pp->element[i].flags & HYPERLINK_ELEMENT_FLAG_NO_HOVER);
}

void hyperlink_page_logic(HYPERLINK_PAGE * pp)
{
	int i;
	
	if(pp)
	{
		pp->hover_element = -1;
		for(i = 0; i < pp->elements; i++)
		{
			if(hyperlink_check_hover(pp, i))
			{
				pp->hover_element = i;
				break;
			}
		}
		if(t3f_mouse_button[0] && !t3f_hyperlink_left_clicked && pp->hover_element >= 0)
		{
			if(pp->element[pp->hover_element].proc)
			{
				pp->element[pp->hover_element].proc(0, pp);
			}
			t3f_hyperlink_left_clicked = true;
		}
		if(!t3f_mouse_button[0])
		{
			t3f_hyperlink_left_clicked = false;
		}
	}
}

void hyperlink_page_render_element(HYPERLINK_PAGE * pp, int i, bool hover)
{
	hyperlink_current_driver->render_element(pp, i, hover);
}

void hyperlink_page_render(HYPERLINK_PAGE * pp)
{
	int i;
	
	if(pp)
	{
		for(i = 0; i < pp->elements; i++)
		{
			hyperlink_page_render_element(pp, i, i == pp->hover_element);
		}
		
		/* render the hover element last so it appears on top */
//		if(pp->hover_element >= 0 && pp->hover_element < pp->elements)
//		{
//			hyperlink_page_render_element(pp, i, true);
//		}
	}
}
