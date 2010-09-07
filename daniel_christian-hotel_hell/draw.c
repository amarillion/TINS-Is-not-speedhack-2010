#define DRAWINGS_FILE		"drawings.txt"
#define Z_SCALE			1

#define SCALE_X			255.0
#define SCALE_Y			255.0

#define LINE_RETRIES		5
#define LINE_ACCURACY		6
#define LINE_WEIGHT		128
#define POINT_ACCURACY	(((float)rand()/(float)RAND_MAX)*3)-1.5
#define PAINT_ACCURACY		(((float)rand()/(float)RAND_MAX)*8.0)-4


typedef struct polytype{
	int r,g,b,a,pts;
	int *pt;
}polytype;

typedef struct drawingtype{
	int num_polys;
	polytype *poly;
} drawingtype;

typedef struct drawingcollectiontype{
	int num_drawings;
	drawingtype *drawing;
}drawingcollectiontype;


drawingcollectiontype *load_drawing_collection( char *filename ){
	int i,j,k;
	char *str;
	char *tok;
	char *tok_r;
	drawingcollectiontype *r;
	
	r = malloc(sizeof(drawingcollectiontype));
	str = string_from_file( filename);
	r->num_drawings = read_number(str,&tok_r);
	
	
	r->drawing = malloc(sizeof(drawingtype) * r->num_drawings );
	for ( i = 0; i < r->num_drawings; i++ ){		
		r->drawing[i].num_polys = read_number(NULL,&tok_r);
		r->drawing[i].poly = malloc(sizeof(polytype) * r->drawing[i].num_polys );
		for ( j = 0; j < r->drawing[i].num_polys; j++ ){

			r->drawing[i].poly[j].r = read_number(NULL,&tok_r);
			r->drawing[i].poly[j].g = read_number(NULL,&tok_r);
			r->drawing[i].poly[j].b= read_number(NULL,&tok_r);
			r->drawing[i].poly[j].a = read_number(NULL,&tok_r);
			r->drawing[i].poly[j].pts = read_number(NULL,&tok_r);
			r->drawing[i].poly[j].pt = malloc(sizeof(int) * r->drawing[i].poly[j].pts );
			for ( k = 0; k < r->drawing[i].poly[j].pts; k++ ){
				r->drawing[i].poly[j].pt[k] = read_number(NULL,&tok_r);
			}
		}
	}	
	free(str);
	return r;
}



// DRAW PRIMATIVE ROUTINGS //
void messy_line( BITMAP *bmp, float x1, float y1, float x2, float y2, int c){
	int i = 0;
	
	set_write_alpha_blender();
	drawing_mode(DRAW_MODE_TRANS,0,0,0);
	
	set_trans_blender(0,0,0,LINE_WEIGHT);
	for ( i = 0; i < LINE_RETRIES; i++ ){
		line( bmp, x1+POINT_ACCURACY, y1+POINT_ACCURACY,x2+POINT_ACCURACY, y2+POINT_ACCURACY, c );	
	}
	drawing_mode(DRAW_MODE_SOLID,0,0,0);
	
}


void offshoot_line( BITMAP *bmp, float ix1, float iy1, float ix2, float iy2, int c){
	float x1, y1,x2,y2;
	float a;
	
	if ( ix1 > ix2 ){
		x1 = ix2;
		x2 = ix1;
		y1 = iy2;
		y2 = iy1;
	} else {
		x1 = ix1;
		x2 = ix2;
		y1 = iy1;
		y2 = iy2;
	}	
	a = atan(  (y2-y1)/(x2-x1) );
	
	x1 -= cos(a) * LINE_ACCURACY;
	y1 -= sin(a) * LINE_ACCURACY;
	x2 += cos(a) * LINE_ACCURACY;
	y2 += sin(a) * LINE_ACCURACY;
		
	
	messy_line( bmp, x1,y1,x2,y2,c);
}


void messy_line_3d( BITMAP *bmp, float x1, float y1, float z1, float x2, float y2, float z2, int c ){
	int half_x = bmp->w/2;
	int half_y = bmp->h/2;
	
	offshoot_line( bmp, (x1/z1)+half_x, (y1/z1)+half_y, (x2/z2)+half_x,(y2/z2)+half_y, c );
}

void messy_polygon( BITMAP *bmp, int pts, int *pt, int c ){
	if ( pts >= 4 ){
		int i;
		for ( i = 2; i < pts;i += 2){
			messy_line( bmp, pt[i-2],pt[i-1], pt[ i ], pt[ i + 1 ], c );
		}
	}	
}



// DRAW STRUCTURE ROUTINES //
void color_poly( BITMAP *bmp, polytype *poly, float x, float y, float z ){
	float half_w=bmp->w/2;
	float half_h=bmp->h/2;
	int i ;
	int *pt;
	pt = malloc(sizeof(int) * poly->pts );
	
	set_trans_blender( 0,0,0, poly->a );
	for ( i = 0; i < poly->pts; i++ ){
		pt[i] = (((((float)(poly->pt[i]-128)/128.0) + x )/(z*Z_SCALE))*half_w)+half_w + PAINT_ACCURACY;
		i++;
		pt[i] = (((((float)(poly->pt[i]-128)/128.0) + y )/(z*Z_SCALE))*half_h)+half_h + PAINT_ACCURACY;
	}
	
	polygon( bmp, poly->pts/2, pt, makecol(poly->r,poly->g,poly->b) );	
	free(pt);
}



void draw_poly( BITMAP *bmp, polytype *poly, float x, float y, float z ){
	int i ;
	float half_w = bmp->w/2;
	float half_h = bmp->h/2;
	int *pt;
	pt = malloc(sizeof(int) * poly->pts );
	
	for ( i = 0; i < poly->pts; i++ ){
		pt[i] = (((((float)(poly->pt[i]-128)/128.0) + x )/(z*Z_SCALE))*half_w)+half_w + PAINT_ACCURACY;
		i++;
		pt[i] = (((((float)(poly->pt[i]-128)/128.0) + y )/(z*Z_SCALE))*half_h)+half_h + PAINT_ACCURACY;
	}	
	
	messy_polygon( bmp, poly->pts, pt, makecol(0,0,0) );	
	drawing_mode(DRAW_MODE_SOLID,0,0,0);
	free(pt);
	
}

void color_poly_sideways( BITMAP *bmp, polytype *poly, float x, float y, float z ){
	float half_w=bmp->w/2;
	float half_h=bmp->h/2;
	int i ;
	int *pt;
	pt = malloc(sizeof(int) * poly->pts );
	
	set_trans_blender( 0,0,0, poly->a );
	for ( i = 0; i < poly->pts; i++ ){
		float nz = (float)((poly->pt[i]-128)/128.0)+z;
		pt[i] = ((x/nz)*half_w)+half_w + PAINT_ACCURACY;
		pt[i] = min(bmp->w,max(0,pt[i]));
		if (( pt[i] < -16 ) || (pt[i] > bmp->w+16)  ) return;
		i++;
		pt[i] = (((((float)(poly->pt[i]-128)/128.0) + y )/nz)*half_h)+half_h + PAINT_ACCURACY;
		pt[i] = min(bmp->h,max(0,pt[i]));
		if (( pt[i] < -16 ) || (pt[i] > bmp->h+16)  ) return;
	}
	
	polygon( bmp, poly->pts/2, pt, makecol(poly->r,poly->g,poly->b) );	
	free(pt);
}



void draw_poly_sideways( BITMAP *bmp, polytype *poly, float x, float y, float z ){
	int i ;
	float half_w = bmp->w/2;
	float half_h = bmp->h/2;
	int *pt;
	pt = malloc(sizeof(int) * poly->pts );
	
	for ( i = 0; i < poly->pts; i++ ){
		float nz = (float)((poly->pt[i]-128)/128.0)+z;
		pt[i] = ((x/nz)*half_w)+half_w + PAINT_ACCURACY;
		pt[i] = min(bmp->w,max(0,pt[i]));
		if (( pt[i] < -16 ) || (pt[i] > bmp->w+16)  ) return;
		i++;
		pt[i] = (((((float)(poly->pt[i]-128)/128.0) + y )/nz)*half_h)+half_h + PAINT_ACCURACY;
		pt[i] = min(bmp->h,max(0,pt[i]));
		if (( pt[i] < -16 ) || (pt[i] > bmp->h+16)  ) return;
	}	
	
	messy_polygon( bmp, poly->pts, pt, makecol(0,0,0) );	
	drawing_mode(DRAW_MODE_SOLID,0,0,0);
	free(pt);
	
}


void color_poly_sideways2( BITMAP *bmp, polytype *poly, float x, float y, float z ){
	float half_w=bmp->w/2;
	float half_h=bmp->h/2;
	int i ;
	int *pt;
	pt = malloc(sizeof(int) * poly->pts );
	
	set_trans_blender( 0,0,0, poly->a );
	for ( i = 0; i < poly->pts; i++ ){
		float nz = (1 - (float)((poly->pt[i]-128)/128.0))+z-1;
		pt[i] = ((x/nz)*half_w)+half_w + PAINT_ACCURACY;
		pt[i] = min(bmp->w,max(0,pt[i]));
		if (( pt[i] < -16 ) || (pt[i] > bmp->w+16)  ) return;
		i++;
		pt[i] = (((((float)(poly->pt[i]-128)/128.0) + y )/nz)*half_h)+half_h + PAINT_ACCURACY;
		pt[i] = min(bmp->h,max(0,pt[i]));
		if (( pt[i] < -16 ) || (pt[i] > bmp->h+16)  ) return;
	}
	
	polygon( bmp, poly->pts/2, pt, makecol(poly->r,poly->g,poly->b) );	
	free(pt);
}



void draw_poly_sideways2( BITMAP *bmp, polytype *poly, float x, float y, float z ){
	int i ;
	float half_w = bmp->w/2;
	float half_h = bmp->h/2;
	int *pt;
	pt = malloc(sizeof(int) * poly->pts );
	
	for ( i = 0; i < poly->pts; i++ ){
		float nz = (1 -(float)((poly->pt[i]-128)/128.0))+z-1;
		pt[i] = ((x/nz)*half_w)+half_w + PAINT_ACCURACY;
		pt[i] = min(bmp->w,max(0,pt[i]));
		if (( pt[i] < -16 ) || (pt[i] > bmp->w+16)  ) return;
		i++;
		pt[i] = (((((float)(poly->pt[i]-128)/128.0) + y )/nz)*half_h)+half_h + PAINT_ACCURACY;
		pt[i] = min(bmp->h,max(0,pt[i]));
		if (( pt[i] < -16 ) || (pt[i] > bmp->h+16)  ) return;
	}	
	
	messy_polygon( bmp, poly->pts, pt, makecol(0,0,0) );	
	drawing_mode(DRAW_MODE_SOLID,0,0,0);
	free(pt);
	
}


void draw_drawing( BITMAP *bmp, drawingtype *drawing, float x, float y, float z ){
	int i ;
	
	if ( z > 0 ){
		drawing_mode(DRAW_MODE_TRANS,0,0,0);
		
		for ( i = 0; i < drawing->num_polys; i++ ){
			 color_poly( bmp, &drawing->poly[i], x, y, z );
		}
		for ( i = 0; i < drawing->num_polys; i++ ){
			draw_poly( bmp, &drawing->poly[i], x, y, z );
		}
		
		drawing_mode(DRAW_MODE_SOLID,0,0,0);
	}
}

void draw_drawing_sideways( BITMAP *bmp, drawingtype *drawing, float x, float y, float z ){
	int i ;
	
	drawing_mode(DRAW_MODE_TRANS,0,0,0);
	
	for ( i = 0; i < drawing->num_polys; i++ ){
		color_poly_sideways( bmp, &drawing->poly[i], x, y, z );
	}
	for ( i = 0; i < drawing->num_polys; i++ ){
		draw_poly_sideways( bmp, &drawing->poly[i], x, y, z );
	}
	
	drawing_mode(DRAW_MODE_SOLID,0,0,0);
}void draw_drawing_sideways2( BITMAP *bmp, drawingtype *drawing, float x, float y, float z ){
	int i ;
	
	drawing_mode(DRAW_MODE_TRANS,0,0,0);
	
	for ( i = 0; i < drawing->num_polys; i++ ){
		color_poly_sideways2( bmp, &drawing->poly[i], x, y, z );
	}
	for ( i = 0; i < drawing->num_polys; i++ ){
		draw_poly_sideways2( bmp, &drawing->poly[i], x, y, z );
	}
	
	drawing_mode(DRAW_MODE_SOLID,0,0,0);
}






void destroy_drawing( drawingtype *d ){
	int i;
	for ( i = 0; i < d->num_polys; i++ ){
		free( d->poly[i].pt );
	}
	free(d->poly );
}
void destroy_drawing_collection( drawingcollectiontype *d ){
	int i;
	for ( i = 0; i < d->num_drawings; i++ ){
		destroy_drawing( &d->drawing[i] );
	}
	free( d->drawing );
	free(d);
}




void messy_box( BITMAP *bmp, double x1, double y1, double x2, double y2, double z, int c){
	//top
	messy_line_3d( bmp,	 x1,y1,z,		x2,y1,z, c);
	//bottom
	messy_line_3d( bmp, 	x1,y2,z,		x2,y2,z, c);
	//left
	messy_line_3d( bmp,	 x1,y1,z,		x1,y2,z, c);
	//right
	messy_line_3d( bmp,	 x2,y1,z,		x2,y2,z, c);
}


