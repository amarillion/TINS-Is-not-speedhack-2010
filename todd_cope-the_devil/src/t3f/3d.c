#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include "t3f.h"
#include "3d.h"

/* This module contains basic functions for projecting 3D points for use on a
   2D surface. A little note: the two functions below you should use the same
   pw and ph if you want the scaling to be 1:1, if you use a 3:4 or something
   else, the object will scale oddly. */

/* projects 3d point onto 2d plane (x)
   x is the x-coordinate of the point in space
   z is the z-coordinate of the point in space
   px is the center of the 2d plane
   pw is the width of the 2d plane */
   
/* internal state of the 3d engine */
static T3F_3D_STATE * t3f_3d_current_state = NULL;
static ALLEGRO_TRANSFORM t3f_3d_transform;

T3F_3D_STATE t3f_3d_state;

void t3f_3d_set_projection(T3F_3D_STATE * sp, float ox, float oy, float w, float h, float vpx, float vpy)
{
	sp->offset_x = ox;
	sp->offset_y = oy;
	sp->width = w;
	sp->height = h;
//	sp->scale_x = sx;
//	sp->scale_y = sy;
	sp->vp_x = vpx;
	sp->vp_y = vpy;
//	sp->view_w = vw;
//	sp->view_h = vh;
}

void t3f_3d_store_state(T3F_3D_STATE * sp)
{
	memcpy(sp, t3f_3d_current_state, sizeof(T3F_3D_STATE));
}

void t3f_3d_restore_state(T3F_3D_STATE * sp)
{
	memcpy(t3f_3d_current_state, sp, sizeof(T3F_3D_STATE));
}

void t3f_3d_select_projection(T3F_3D_STATE * sp)
{
	t3f_3d_current_state = sp;
	float sx = t3f_3d_current_state->width / (float)t3f_virtual_display_width;
	float sy = t3f_3d_current_state->height / (float)t3f_virtual_display_height;
	float dsx = (float)al_get_display_width(t3f_display) / t3f_virtual_display_width;
	float dsy = (float)al_get_display_height(t3f_display) / t3f_virtual_display_height;
	float ox = t3f_3d_current_state->offset_x * dsx;
	float oy = t3f_3d_current_state->offset_y * dsy;
	
	/* apply additional transformations */
	al_copy_transform(&t3f_current_transform, &t3f_base_transform);
	al_scale_transform(&t3f_current_transform, sx, sy);
	al_translate_transform(&t3f_current_transform, t3f_3d_current_state->offset_x * dsx, t3f_3d_current_state->offset_y * dsy);
	al_use_transform(&t3f_current_transform);
	
	/* convert virtual screen coordinates to real display coordinates */
	al_set_clipping_rectangle(ox, oy, t3f_3d_current_state->width * dsx, t3f_3d_current_state->height * dsy);
}

/* get the x coordinate of the pixel at the given (x, z) coordinate
   takes current projection state into account */
float t3f_3d_project_x(float x, float z)
{
	float rx;
	
	if(z + t3f_3d_current_state->width > 0)
	{
//		rx = t3f_3d_current_state->scale_x * (((x - t3f_3d_current_state->vp_x) * t3f_3d_current_state->view_w) / (z + t3f_3d_current_state->view_w) + t3f_3d_current_state->vp_x) + t3f_3d_current_state->offset_x;
		rx = (((x - t3f_3d_current_state->vp_x) * t3f_3d_current_state->width) / (z + t3f_3d_current_state->width) + t3f_3d_current_state->vp_x);
		return rx;
	}
	else
	{
		return -65536;
	}
}

/* get the y coordinate of the pixel at the given (y, z) coordinate
   takes current projection state into account */
float t3f_3d_project_y(float y, float z)
{
	float ry;
	
	if(z + t3f_3d_current_state->height > 0)
	{
//		ry = t3f_3d_current_state->scale_y * (((y - t3f_3d_current_state->vp_y) * t3f_3d_current_state->view_h) / (z + t3f_3d_current_state->view_h) + t3f_3d_current_state->vp_y) + t3f_3d_current_state->offset_y;
		ry = (((y - t3f_3d_current_state->vp_y) * t3f_3d_current_state->width) / (z + t3f_3d_current_state->width) + t3f_3d_current_state->vp_y);
		return ry;
	}
	else
	{
		return -65536;
	}
}

/* set up a transformation such that anything drawn will be as if its 'z'
 * coordinate was at the specified 'z' */
void t3f_3d_transform_z(float z)
{
	float scale = t3f_3d_project_x(1.0, z) - t3f_3d_project_x(0.0, z);
	float offset_x = t3f_3d_project_x(0.0, z);
	float offset_y = t3f_3d_project_y(0.0, z);
	al_identity_transform(&t3f_3d_transform);
	al_scale_transform(&t3f_3d_transform, scale, scale);
	al_translate_transform(&t3f_3d_transform, offset_x, offset_y);
//	al_transform_transform(&t3f_3d_transform, &t3f_current_transform);
	al_use_transform(&t3f_3d_transform);
}

void t3f_3d_draw_line(float x, float y, float z, float x2, float y2, float z2, ALLEGRO_COLOR color)
{
	float obx, oby, oex, oey;
	
	obx = t3f_3d_project_x(x, z);
	oex = t3f_3d_project_x(x2, z2);
	oby = t3f_3d_project_y(y, z);
	oey = t3f_3d_project_y(y2, z2);
	
	al_draw_line(obx, oby, oex, oey, color, 1.0);
}

void t3f_3d_draw_bitmap(ALLEGRO_BITMAP * bp, ALLEGRO_COLOR color, float x, float y, float z, int flags)
{
	/* upper left and bottom right points in 3d */
	float obj_x[2], obj_y[2], obj_z[2];
	
	/* upper left and bottom right points in 2d */
	float screen_w, screen_h;

	obj_x[0] = t3f_3d_project_x(x, z);
	obj_x[1] = t3f_3d_project_x(x + al_get_bitmap_width(bp), z);
	obj_y[0] = t3f_3d_project_y(y, z);
	obj_y[1] = t3f_3d_project_y(y + al_get_bitmap_height(bp), z);
	obj_z[0] = z + t3f_3d_current_state->width;
	obj_z[1] = z + t3f_virtual_display_width;
	
	/* clip sprites at z = 0 */
	if(obj_z[0] > 0)
	{
		screen_w = obj_x[1] - obj_x[0];
		screen_h = obj_y[1] - obj_y[0];
		al_draw_tinted_scaled_bitmap(bp, color, 0, 0, al_get_bitmap_width(bp), al_get_bitmap_height(bp), obj_x[0], obj_y[0], screen_w, screen_h, flags);
	}
}

void t3f_3d_draw_rotated_bitmap(ALLEGRO_BITMAP * bp, ALLEGRO_COLOR color, float cx, float cy, float x, float y, float z, float angle, int flags)
{
	/* upper left and bottom right points in 3d */
	float obj_x[2], obj_y[2], obj_z[2], obj_cx, obj_cy;
	float rx, ry;
	
	/* upper left and bottom right points in 2d */
	float screen_w, screen_h;

	obj_x[0] = t3f_3d_project_x(x - cx, z);
	obj_x[1] = t3f_3d_project_x(x + cx, z);
	obj_y[0] = t3f_3d_project_y(y - cy, z);
	obj_y[1] = t3f_3d_project_y(y + cy, z);
	obj_z[0] = z + t3f_3d_current_state->width;
	obj_z[1] = z + t3f_virtual_display_width;
	obj_cx = t3f_3d_project_x(x, z);
	obj_cy = t3f_3d_project_y(y, z);
	
	/* clip sprites at z = 0 */
	if(obj_z[0] > 0)
	{
		screen_w = obj_x[1] - obj_x[0];
		screen_h = obj_y[1] - obj_y[0];
		rx = screen_w / al_get_bitmap_width(bp);
		ry = screen_h / al_get_bitmap_height(bp);
		al_draw_tinted_rotated_scaled_bitmap(bp, color, cx, cy, obj_cx, obj_cy, rx, ry, angle, flags);
	}
}

void t3f_3d_draw_rotated_scaled_bitmap(ALLEGRO_BITMAP * bp, ALLEGRO_COLOR color, float cx, float cy, float x, float y, float z, float angle, float scale_x, float scale_y, int flags)
{
	/* upper left and bottom right points in 3d */
	float obj_x[2], obj_y[2], obj_z[2], obj_cx, obj_cy;
	float rx, ry;
	
	/* upper left and bottom right points in 2d */
	float screen_w, screen_h;

	obj_x[0] = t3f_3d_project_x(0, z);
	obj_x[1] = t3f_3d_project_x((float)al_get_bitmap_width(bp) * scale_x, z);
	obj_y[0] = t3f_3d_project_y(0, z);
	obj_y[1] = t3f_3d_project_y((float)al_get_bitmap_height(bp) * scale_y, z);
	obj_z[0] = z + t3f_3d_current_state->width;
	obj_z[1] = z + t3f_virtual_display_width;
	obj_cx = t3f_3d_project_x(x, z);
	obj_cy = t3f_3d_project_y(y, z);
	
	/* clip sprites at z = 0 */
	if(obj_z[0] > 0)
	{
		screen_w = obj_x[1] - obj_x[0];
		screen_h = obj_y[1] - obj_y[0];
		rx = screen_w / al_get_bitmap_width(bp);
		ry = screen_h / al_get_bitmap_height(bp);
		al_draw_tinted_rotated_scaled_bitmap(bp, color, cx, cy, obj_cx, obj_cy, rx, ry, angle, flags);
	}
}

void t3f_3d_draw_scaled_bitmap(ALLEGRO_BITMAP * bp, ALLEGRO_COLOR color, float x, float y, float z, float w, float h, int flags)
{
	/* upper left and bottom right points in 3d */
	float obj_x[2], obj_y[2], obj_z[2];
	
	/* upper left and bottom right points in 2d */
	float screen_w, screen_h;

	obj_x[0] = t3f_3d_project_x(x, z);
	obj_x[1] = t3f_3d_project_x(x + w, z);
	obj_y[0] = t3f_3d_project_y(y, z);
	obj_y[1] = t3f_3d_project_y(y + h, z);
	obj_z[0] = z + t3f_3d_current_state->width;
	obj_z[1] = z + t3f_virtual_display_width;
	
	/* clip sprites at z = 0 */
	if(obj_z[0] > 0)
	{
		screen_w = obj_x[1] - obj_x[0];
		screen_h = obj_y[1] - obj_y[0];
		al_draw_tinted_scaled_bitmap(bp, color, 0, 0, al_get_bitmap_width(bp), al_get_bitmap_height(bp), obj_x[0], obj_y[0], screen_w, screen_h, flags);
	}
}
