#ifndef T3F_3D_H
#define T3F_3D_H

#include <allegro5/allegro5.h>

#ifdef __cplusplus
   extern "C" {
#endif

/* structure holds information about a 3D viewport usually used to represent
   one player's screen, split screen games will have multiple viewports */
typedef struct
{
	
	/* offset of viewport */
	float offset_x;
	float offset_y;
	float width;
	float height;
	
	/* scale the final output by this much */
//	float scale_x;
//	float scale_y;
	
	/* vanishing point */
	float vp_x;
	float vp_y;
	
	/* size of the viewport */
//	float view_w;
//	float view_h;	

} T3F_3D_STATE;

extern T3F_3D_STATE t3f_3d_state;

/* state setting functions, use states so you don't have to keep passing
   the same arguments to the other functions over and over */
void t3f_3d_set_projection(T3F_3D_STATE * sp, float ox, float oy, float w, float h, float vpx, float vpy);
void t3f_3d_store_state(T3F_3D_STATE * sp);
void t3f_3d_restore_state(T3F_3D_STATE * sp);
void t3f_3d_select_projection(T3F_3D_STATE * sp);

float t3f_3d_project_x(float x, float z);
float t3f_3d_project_y(float y, float z);
void t3f_3d_transform_z(float z);
void t3f_3d_draw_line(float x, float y, float z, float x2, float y2, float z2, ALLEGRO_COLOR color);
void t3f_3d_draw_bitmap(ALLEGRO_BITMAP * bp, ALLEGRO_COLOR color, float x, float y, float z, int flags);
void t3f_3d_draw_rotated_bitmap(ALLEGRO_BITMAP * bp, ALLEGRO_COLOR color, float cx, float cy, float x, float y, float z, float angle, int flags);
void t3f_3d_draw_rotated_scaled_bitmap(ALLEGRO_BITMAP * bp, ALLEGRO_COLOR color, float cx, float cy, float x, float y, float z, float angle, float scale_x, float scale_y, int flags);
void t3f_3d_draw_scaled_bitmap(ALLEGRO_BITMAP * bp, ALLEGRO_COLOR color, float x, float y, float z, float w, float h, int flags);

#ifdef __cplusplus
   }
#endif

#endif
