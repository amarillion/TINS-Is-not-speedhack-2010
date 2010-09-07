

#define EAST		0
#define SOUTH	1
#define WEST		2
#define NORTH		3
#define SPRITE_WALL	4

#define PI 3.14159265

#define RAY_START_ANGLE	-90.0
#define RAY_END_ANGLE		90.0
#define RAYS_TO_CAST		160


typedef struct tiletype{
	int wall[5];	// n, e, s, w, sprite
} tiletype;

typedef struct mazetype{
	int width;
	int height;
	tiletype **tile;
} mazetype;

typedef struct caster{
	int width;
	int height;
	int **map;
} caster;

mazetype *create_maze( int w, int h ){
	mazetype *m = malloc(sizeof(mazetype));
	int x,y,wall;
	m->width = w;
	m->height = h;
	
	wall = 0;
	m->tile = malloc(sizeof(tiletype *)*w);
	for ( x = 0; x < w; x++ ){
		m->tile[x] = malloc(sizeof(tiletype) * h);
		for ( y = 0; y < h; y++ ){		
			for ( wall = 0; wall < 5; wall++ ){
				m->tile[x][y].wall[wall] = 0;
			}
		}
	}
	return m;
}


void destroy_maze( mazetype *m ){
	int x;
	for ( x = 0; x < m->height; x++ ){
		free(m->tile[x]);
	}
	free(m->tile);
	free(m);
}


void save_maze( mazetype *maze, char *filename){
	int x,y, w;
	FILE *f = fopen(filename,"w");
	
	fprintf(f,"%i,%i,",maze->width,maze->height );
	for ( y = 0; y < maze->height; y++ ){
		for ( x = 0; x < maze->width; x++ ){
			for ( w = 0; w < 5; w++ ){
				fprintf(f,"%i,",maze->tile[x][y].wall[w]);
			}
		}
	}		
	fclose(f);
}

mazetype *load_maze( char *filename ){
	int x,y,w;
	char *str;
	char *tok_r;
	int width,height;
	
	str = string_from_file(filename);
	
	width = read_number(str,&tok_r);
	height = read_number(NULL,&tok_r);
	mazetype *m = create_maze( width, height);
	for ( y = 0; y < m->height; y++ ){
		for ( x = 0; x < m->width; x++ ){
			for ( w = 0; w < 5; w++ ){
				m->tile[x][y].wall[w] = read_number(NULL,&tok_r);
			}
		}
	}
	
	free(str);
	return m;
}


void clear_caster( caster *c ){
	int x,y,i;
	for ( x = 0; x < c->width; x++){
		for ( y = 0; y < c->height; y++ ){
			c->map[x][y] = 0;
		}
	}
}

caster *new_caster( int width, int height ){
	int i;
	caster *c = malloc(sizeof(caster));
	c->width = width;
	c->height = height;
	c->map = malloc(sizeof(int *)*width);
	for ( i = 0; i < width; i++ ){
		c->map[i] = malloc(sizeof(int) * height );
	}
	clear_caster(c);
	return c;
}

void destroy_caster( caster *c ){
	int i;
	for ( i = 0; i < c->width; i++ ){
		free( c->map[i] );
	}
	free(c->map);
	free(c);
}


void cast_ray( mazetype *m, caster *c, float x, float y, float step, float angle ){
	double ray_x = x;
	double ray_y = y;
	double step_x = cos(angle) * step;
	double step_y = sin(angle) * step;
	
	int done = 0;

	while (!done && !done){
		
		int ox = floor(ray_x);
		int oy  = floor(ray_y);
		int nx = floor(ray_x+step_x);
		int ny = floor(ray_y+step_y);
		
		c->map[ox][oy] = 1;

		
		if ( ( nx < 0 ) || ( nx >= c->width) || ( ny < 0 ) || ( ny >= c->height) ){
			done = 1;
		} else {
			
			//check north and south walls
			if ( ox == nx ){
				if ( ( (oy-ny) == 1 ) && (m->tile[ox][oy].wall[NORTH] > 0 ) ){
					done=1;
				} else if ( ( (ny-oy) == 1) && (m->tile[ox][oy].wall[ SOUTH ] > 0 ) ){
					done=1;
				}				
			}
			

			//check west and east walls
			if ( oy == ny ){
				if ( ( (ox-nx) == 1 ) && (m->tile[ox][oy].wall[WEST] > 0 ) ){
					done=1;
				} else if ( ( (nx-ox) == 1) && (m->tile[ox][oy].wall[ EAST ] > 0 ) ){
					done=1;
				}				
			}
			if (( ox != nx ) && (oy != ny ) ){
				done = 1;
			}
			
			
		}
		ray_x += step_x;
		ray_y += step_y;
	}
	
	
}

void cast_ray_fan( mazetype *m, caster *c, float x, float y, float step, float angle ){
	float angle_step = ((RAY_END_ANGLE - RAY_START_ANGLE) / RAYS_TO_CAST);
	float a;
	
	//~ printf("angle step:%f\n",(double)angle_step);
	clear_caster(c);
	for ( a = angle+RAY_START_ANGLE; a < angle+RAY_END_ANGLE; a += angle_step ){
		cast_ray( m, c, x, y,step, (a * PI)/180 );
	}	
}

typedef void (*box_drawer_type)(BITMAP *, drawingcollectiontype *, tiletype *, float x, float y, float z );
box_drawer_type box_drawer[4];

typedef int (*caster_reader_type)( caster *, float *, float *, float, float, float, float );
caster_reader_type caster_reader[4];

void draw_box_east( BITMAP *bmp, drawingcollectiontype *dc, tiletype *t, float x, float y, float z ){
	//draw the far wall dead-on (EAST)
	int i = t->wall[EAST];
	if ( i > 0 ){
		draw_drawing(	bmp, &dc->drawing[i], x,y,z+1 );
	}

	
	//draw the left wall if it's not out of bounds (NORTH)
	i = t->wall[NORTH];
	if ( i > 0 ){
		draw_drawing_sideways( bmp, &dc->drawing[i], x-1, y, z );
	}
	
	//draw the right  wall if it's not out of bounds (SOUTH)
	i = t->wall[SOUTH];
	if ( i > 0 ){
		draw_drawing_sideways2( bmp, &dc->drawing[i], x+1, y, z );
	}
	//draw the closer wall dead-on (WEST)
	//~ i = t->wall[WEST];
	//~ if ( i > 0 ){
		//~ draw_drawing(	bmp, &dc->drawing[i], x,y,z-1 );
	//~ }	
}

void draw_box_south( BITMAP *bmp, drawingcollectiontype *dc, tiletype *t, float x, float y, float z ){
	//draw the far wall dead-on (SOUTH)
	int i = t->wall[SOUTH];
	if ( i > 0 ){
		draw_drawing(	bmp, &dc->drawing[i], x,y,z+1 );
	}
	

	
	//draw the left wall if it's not out of bounds (EAST)
	i = t->wall[EAST];
	if ( i > 0 ){
		draw_drawing_sideways( bmp, &dc->drawing[i], x-1, y, z );
	}
	
	//draw the right wall if it's not out of bounds (WEST)
	i = t->wall[WEST];
	if ( i > 0 ){
		draw_drawing_sideways2( bmp, &dc->drawing[i], x+1, y, z );
	}
	
	//draw the closer wall dead-on (NORTH)
	//~ i = t->wall[NORTH];
	//~ if ( i > 0 ){
		//~ draw_drawing(	bmp, &dc->drawing[i], x,y,z-1 );
	//~ }	
}

void draw_box_west( BITMAP *bmp, drawingcollectiontype *dc, tiletype *t, float x, float y, float z ){
	//draw the far wall dead-on (WEST)
	int i = t->wall[WEST];
	if ( i > 0 ){
		draw_drawing(	bmp, &dc->drawing[i], x,y,z+1 );
	}

	
	//draw the left wall if it's not out of bounds (SOUTH)
	i = t->wall[SOUTH];
	if ( i > 0 ){
		draw_drawing_sideways( bmp, &dc->drawing[i], x-1, y, z );	
	}
	//draw the right wall if it's not out of bounds (NORTH)
	i = t->wall[NORTH];
	if ( i > 0 ){
		draw_drawing_sideways2( bmp, &dc->drawing[i], x+1, y, z );
	}

	//draw the closer wall dead-on (EAST)
	//~ i = t->wall[EAST];
	//~ if ( i > 0 ){
		//~ draw_drawing(	bmp, &dc->drawing[i], x,y,z-1 );
	//~ }	
 }

void draw_box_north( BITMAP *bmp, drawingcollectiontype *dc, tiletype *t, float x, float y, float z ){
	//draw the far wall dead-on (NORTH)
	int i = t->wall[NORTH];
	if ( i > 0 ){
		draw_drawing(	bmp, &dc->drawing[i], x,y,z+1 );
	}

	
	//draw the left wall if it's not out of bounds (WEST)
	i = t->wall[WEST];
	if ( i > 0 ){
		draw_drawing_sideways( bmp, &dc->drawing[i], x-1, y, z );
	}
	
	//draw the right wall if it's not out of bounds (EAST)
	i = t->wall[EAST];
	if ( i > 0 ){
		draw_drawing_sideways2( bmp, &dc->drawing[i], x+1, y, z );	
	}
	//~ //draw the closer wall dead-on (SOUTH)
	//~ i = t->wall[SOUTH];
	//~ if ( i > 0 ){
		//~ draw_drawing(	bmp, &dc->drawing[i], x,y,z-1 );
	//~ }	
}


int caster_east_reader( caster *c,   float *out_x,  float *out_y, float x, float y, float v, float h ){	
	float ox = x+ v;
	float oy = y + h;
	*out_x = ox;
	*out_y = oy;
	
	return (!( ox < 0 ) && !( ox >= c->width) && !( oy < 0) && !( oy >= c->height)) ? c->map[(int)ox][(int)oy] : 0;
}
int caster_south_reader(  caster *c,  float *out_x, float *out_y, float x, float y, float v, float h ){	
	float ox = x - h;
	float oy = y + v;
	*out_x = ox;
	*out_y = oy;
	return (!( ox < 0 ) && !( ox >= c->width) && !( oy < 0) && !( oy >= c->height)) ? c->map[(int)ox][(int)oy] : 0;
		
}
int caster_west_reader(  caster *c,  float *out_x, float *out_y, float x, float y, float v, float h ){	
	float ox = x- v;
	float oy = y - h;
	*out_x = ox;
	*out_y = oy;
	
	return (!( ox < 0 ) && !( ox >= c->width) && !( oy < 0) && !( oy >= c->height)) ? c->map[(int)ox][(int)oy] : 0;
}
int caster_north_reader( caster *c,  float *out_x, float *out_y, float x, float y, float v, float h ){	
	float ox = x + h;
	float oy = y - v;
	*out_x = ox;
	*out_y = oy;
	
	return (!( ox < 0 ) && !( ox >= c->width) && !( oy < 0) && !( oy >= c->height)) ? c->map[(int)ox][(int)oy] : 0;
}

void init_casters(){
	caster_reader[0] = &caster_east_reader;
	caster_reader[1] = &caster_south_reader;
	caster_reader[2] = &caster_west_reader;
	caster_reader[3] = &caster_north_reader;
	
	box_drawer[0] = &draw_box_east;
	box_drawer[1] = &draw_box_south;
	box_drawer[2] = &draw_box_west;
	box_drawer[3] = &draw_box_north;
	
	
}

void draw_caster( BITMAP *bmp, mazetype *m, caster *c, drawingcollectiontype *dc, float x, float y, int direction, int depth){
	//run the ray caster
	int i,h;
	clear_caster(c);
	
	cast_ray_fan(m,c,x,y,0.5, direction * 90);

	//draw in a triangle pattern from back to front
	for ( i = depth; i >= 0; i-- ){
		int sh = -(depth/2);
		int eh = (depth/2);
		for ( h = sh; h < eh; h++ ){
			float ox,oy;
			int t;
			t = caster_reader[direction]( c, &ox, &oy, x, y, i, h );
			if ( t > 0 ){
				int s = m->tile[(int)ox][(int)oy].wall[SPRITE_WALL];
				box_drawer[direction]( bmp, dc, &m->tile[(int)ox][(int)oy], h*2,0,(i*2)+1.1);
				if ( s > 0 ){
					draw_drawing( bmp, &dc->drawing[s], h*2, 0, (i*2)+0.6 );
				}
			}
			
		}
	}
}
//(BITMAP *, drawingcollectiontype *, tiletype *, float x, float y, float z );

	
//~ }


