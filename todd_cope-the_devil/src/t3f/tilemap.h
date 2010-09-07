#ifndef T3F_TILEMAP_H
#define T3F_TILEMAP_H

#include <allegro5/allegro5.h>
#include "3d.h"
#include "animation.h"

#define T3F_MAX_TILES         1024
#define T3F_MAX_LAYERS          32
#define T3F_TILE_MAX_DATA       16
#define T3F_MAX_TILE_SHEETS     16

#define T3F_TILE_FLAG_ANIMATED   1
#define T3F_TILE_FLAG_ONCE       2

#define T3F_TILEMAP_CAMERA_FLAG_NO_TRANSFORM 1

typedef struct
{
	
	T3F_ANIMATION * ap;
	int user_data[T3F_TILE_MAX_DATA];
	int flags;
	
	/* animated tiles (tiles which change to other tiles) */
	short frame_list[1024];
	short frame_list_total;
	
} T3F_TILE;

typedef struct
{
	
	ALLEGRO_BITMAP * tile_sheet[T3F_MAX_TILE_SHEETS];
	int tile_sheets;
	
	T3F_TILE * tile[T3F_MAX_TILES];
	int tiles;
	int width;
	int height;
	int flags;
	
} T3F_TILESET;

typedef struct
{
	
	/* map data */
	short ** data;
	int width;
	int height;
	int tile_sheet;
	
	/* position of layer plane in 3D space */
	float x;
	float y;
	float z;
	
	/* scaling attributes */
	float scale;
	float speed_x;
	float speed_y;
	
	int flags;
	
} T3F_TILEMAP_LAYER;

typedef struct
{
	
	T3F_TILESET * tileset;
	T3F_TILEMAP_LAYER * layer[T3F_MAX_LAYERS];
	int layers;
	
	int flags;
	
} T3F_TILEMAP;

typedef struct
{
	
	float x, y, z;
	int flags;
	
	int vx, vy, vw, vh;
	T3F_3D_STATE projection;
	
} T3F_TILEMAP_CAMERA;

T3F_TILE * t3f_create_tile(void);
short t3f_get_tile(T3F_TILESET * tsp, int tile, int tick);
T3F_TILESET * t3f_create_tileset(int w, int h);
void t3f_destroy_tileset(T3F_TILESET * tsp);
T3F_TILESET * t3f_load_tileset_entry(ALLEGRO_FILE * fp);
T3F_TILESET * t3f_load_tileset(char * fn);
int t3f_save_tileset_entry(T3F_TILESET * tsp, ALLEGRO_FILE * fp);
int t3f_save_tileset(T3F_TILESET * tsp, char * fn);

T3F_TILEMAP_LAYER * t3f_create_tilemap_layer(int w, int h);
void t3f_destroy_tilemap_layer(T3F_TILEMAP_LAYER * tlp);

T3F_TILEMAP * t3f_create_tilemap(int w, int h, int layers);
void t3f_destroy_tilemap(T3F_TILEMAP * tmp);
T3F_TILEMAP * t3f_load_tilemap_entry(ALLEGRO_FILE * fp);
T3F_TILEMAP * t3f_load_tilemap(char * fn);
int t3f_save_tilemap_entry(T3F_TILEMAP * tmp, ALLEGRO_FILE * fp);
int t3f_save_tilemap(T3F_TILEMAP * tmp, char * fn);

void t3f_select_camera(T3F_TILEMAP_CAMERA * cp);
void t3f_render_tilemap(T3F_TILEMAP * tmp, int layer, int tick, T3F_TILEMAP_CAMERA * cp);

#endif
