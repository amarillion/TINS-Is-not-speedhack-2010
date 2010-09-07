#include <time.h>
#include <math.h>
#include <stdio.h>
#include <allegro.h>


#define START_LIFE		100
#define NUMFLOORS	3

#define BLACK			makecol(0,0,0)
#define WHITE			makecol(255,255,255)


#define INTRO_FRAMES	8
#define LOSE_FRAME	8
#define WIN_FRAME		9
#define END_FRAME		11


#include "main.h"
#include "extstring.c"
#include "draw.c"
#include "maze.c"


BITMAP *buffer;
drawingcollectiontype *dc;

void do_intro(){
	int i;
	int k = 0;
	int forward = 0;
	drawingcollectiontype *intro = load_drawing_collection("intro.txt");
	
	for ( i = 0; i < INTRO_FRAMES; i++ ){		
		forward = 0;
		while (!forward){	
			clear_to_color(buffer,WHITE);
			draw_drawing(buffer, &intro->drawing[i],0,0,1 );
			draw_sprite(screen,buffer,0,0);
			k = readkey() >> 8;
			if ( (k == KEY_SPACE) || ( k == KEY_ENTER) || (k == KEY_ESC) ){
				forward = 1;
			}
		}
	}
	destroy_drawing_collection(intro);
}

void do_game_over(){
	int i;
	int k = 0;
	int forward = 0;
	drawingcollectiontype *intro = load_drawing_collection("intro.txt");
	
	while (!forward){	
		clear_to_color(buffer,WHITE);
		draw_drawing(buffer, &intro->drawing[LOSE_FRAME],0,0,1 );
		draw_sprite(screen,buffer,0,0);
		k = readkey() >> 8;
		if ( (k == KEY_SPACE) || ( k == KEY_ENTER) || (k == KEY_ESC) ){
			forward = 1;
		}
	}
	destroy_drawing_collection(intro);	
}

void do_game_win(){
	int i;
	int k = 0;
	int forward = 0;
	drawingcollectiontype *intro = load_drawing_collection("intro.txt");
	
	for ( i = WIN_FRAME; i < END_FRAME; i++ ){		
		forward = 0;
		while (!forward){	
			clear_to_color(buffer,WHITE);
			draw_drawing(buffer, &intro->drawing[i],0,0,1 );
			draw_sprite(screen,buffer,0,0);
			k = readkey() >> 8;
			if ( (k == KEY_SPACE) || ( k == KEY_ENTER) || (k == KEY_ESC) ){
				forward = 1;
			}
		}
	}
	destroy_drawing_collection(intro);
}


int game_timer = 0;
void inc_game_timer(){
	game_timer++;
}
void do_source_anim(){
	char buf[80];
	int i;
	FILE *f = fopen("main.c","r");
	
	clear(screen);
	game_timer = 0;
	for ( i = 0; i < 60; i++ ){
		if ( !feof(f) ){
			fgets( buf, 80, f );
		}
		textout_ex(screen,font,buf,0,i*8,makecol(255,255,255),0);
		
		while ( game_timer == 0 ){
		}
		game_timer--;
	}
	
	fclose(f);
}
void do_game(){
	int i;
	int player_x = 0;
	int player_y = 0;
	int player_direction = 0;
	int done = 0;
	int k = 0;
	int life = START_LIFE;
	int disk_in_hand = 0;
	int s,w;
	int disks_in = 0;
	int player_won = 0;
	
	char *floor[] = {"floor1.txt","floor2.txt","floor3.txt"};
	int current_floor = 0;
	mazetype **maze;
	caster *c;
	
	dc = load_drawing_collection("drawings.txt");	
	maze = malloc(sizeof( mazetype *) * NUMFLOORS);
	for ( i = 0; i < NUMFLOORS; i++ ){
		maze[i] = load_maze( floor[i] );
	}
	c = new_caster( maze[0]->width, maze[0]->height );
	
	
	
	while ( !done ){
		clear_to_color(buffer,WHITE);

		draw_caster( buffer, maze[current_floor], c, dc, player_x+0.5,player_y + 0.5, player_direction, 8 );
		
		//draw life
		for ( i = 0; i < life/10; i++ ){
			draw_drawing( buffer, &dc->drawing[95], -5+i,5,6 );
		}
		//draw disk in hand
		if ( disk_in_hand > 0 ){
			draw_drawing( buffer, &dc->drawing[disk_in_hand], -5+i,5,6 );
		}
		draw_sprite(screen,buffer,0,0);
		k = readkey() >> 8;
		switch ( k ){

			case KEY_UP:			
				life--;
				if ( life == 0 ){
					done = 1;
				}
				
				if ( maze[current_floor]->tile[player_x][player_y].wall[player_direction] == 0 ){

					switch( player_direction){
						case EAST:
								player_x++;
						break;
						case SOUTH:
								player_y++;
						break;
						case WEST:
							player_x--;
						break;
						case NORTH:
							player_y--;
						break;
					}
				}
			break;
			case KEY_LEFT:
				player_direction -= (player_direction == 0 ) ? -3 : 1;
			break;
			case KEY_RIGHT:
				player_direction += (player_direction == 3 ) ? -3 : 1;
			break;
			
			case KEY_SPACE:
				s = maze[current_floor]->tile[player_x][player_y].wall[SPRITE_WALL];
				w = maze[current_floor]->tile[player_x][player_y].wall[ player_direction ];
				if ( s != 0 ){
					//disks
					if ( ( s >= 91 ) && ( s <= 94) ){
						if ( disk_in_hand == 0 ){
							disk_in_hand = s;
							maze[current_floor]->tile[player_x][player_y].wall[SPRITE_WALL] = 0;
						}
					//life
					} else if ( s == 95 ){
						life = START_LIFE;
						maze[current_floor]->tile[player_x][player_y].wall[SPRITE_WALL] = 0;
					}
				//elevator
				} else {
					if (( w >= 31) && (w <= 36)){
						//elevator
						
						//which one is it
						
						//floor 1 at 5,0,east
						if ( ( current_floor == 0) && (player_x == 5) && (player_y == 0 ) && ( player_direction == EAST)){
							current_floor++;
						} else {
							//floor 2 at 5,0 north (goes down)
							if ( ( current_floor == 1) && (player_x == 5) && (player_y == 0 ) && ( player_direction == EAST)){
								current_floor--;
							} else {
								//floor 2, 0,9,S - Up
								if ( ( current_floor == 1) && (player_x == 0) && (player_y == 9 ) && ( player_direction == SOUTH)){
									current_floor++;
								} else {
									//floor 3 0,9,S - Down
									if ( ( current_floor == 2) && (player_x == 0) && (player_y == 9 ) && ( player_direction == SOUTH)){
										current_floor--;
									}

								}
								
							}
						}
						
						//floor x?
						//if ( ( current_floor == 0) && (player_x == 5) && (player_y == 0 ) && ( player_direction == NORTH)){

							
						
					} else {
						if ( (w >= 49) && (w <=54) ){
							//computer
							if ( disk_in_hand > 0 ){
								do_source_anim();
								disk_in_hand = 0;
								disks_in++;
								if ( disks_in == 4 ){
									done = 1;
									player_won = 1;
								}
							}
						} else {
							if ( disk_in_hand > 0 ){
								//drop
								maze[current_floor]->tile[player_x][player_y].wall[SPRITE_WALL] = disk_in_hand;
								disk_in_hand = 0;
							}
						}
					}
				}
			break;
			
			case KEY_ESC:
				done = 1;
			break;
		}
	}
	
	if ( player_won > 0 ){
		do_game_win();
	} else {
		do_game_over();
	}
	
	destroy_drawing_collection(dc);
	destroy_caster(c);
	for ( i = 0; i < NUMFLOORS; i++ ){
		destroy_maze( maze[i]);
	}
	free(maze);
}


int main(int argc, char **argv ){
	int i = 0;
	int k = 0;
	
	
	allegro_init();
	install_keyboard();	
	srand(time(0));
	init_casters();

	set_color_depth(32);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED,640,480,0,0);
	
	buffer = create_bitmap(640,480);
	
	install_int_ex(inc_game_timer,BPS_TO_TIMER(20));
	
	k = 0;
	
	do_intro();
	do_game();
	
	

	
	destroy_bitmap(buffer);
	allegro_exit();
	
	return 0;
	
} END_OF_MAIN();
