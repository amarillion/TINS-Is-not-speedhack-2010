#include "elemsun.h"
#include "datafile.h"
#include <aldumb.h>
#include <jpgalleg.h>

static int white;
static int black;
static int darkGrey;
static int darkGreen;
static int lightGreen;
static int green;
static int buttonFillColour;
static int buttonFillHighlight;
static int darkRed;
static int lightRed;
static int red;
static int darkYellow;

static void setColours() {
	white=makecol(255,255,255);
	black=makecol(0,0,0);
	darkGrey=makecol(85,85,85);
	darkGreen=makecol(60,160,100);
	lightGreen=makecol(96,255,160);
	green=makecol(85,255,85);
	buttonFillColour=makecol(60,90,140);
	buttonFillHighlight=makecol(140,170,220);
	darkRed=makecol(160,90,70);
	lightRed=makecol(255,144,112);
	red=makecol(192,0,0);
	darkYellow=makecol(128,128,0);
}

#define MOUSE_REPEAT_INITIAL 30
#define MOUSE_REPEAT_SUBSEQUENT 4

#define MAP_DRAW_XBASE 80
#define MAP_DRAW_YBASE 80
#define MAP_DRAW_XSTEP 32
#define MAP_DRAW_YSTEP 32

#define LEVEL_LEFT 480
#define LEVEL_RIGHT 624
#define LEVEL_TOP 16
#define LEVEL_BOTTOM 40
#define LEVEL_BUTTON_W 24

#define COLOUR_DRAW_X 480
#define COLOUR_DRAW_YBASE (48-COLOUR_DRAW_YSTEP)
#define COLOUR_DRAW_YSTEP 32

#define BEHAVIOUR_DRAW_LEFT 480
#define BEHAVIOUR_DRAW_RIGHT 624
#define BEHAVIOUR_DRAW_TOP (COLOUR_DRAW_YBASE+COLOUR_DRAW_YSTEP*MAX_COLOURS+4)
#define BEHAVIOUR_DRAW_BOTTOM (BEHAVIOUR_DRAW_TOP+4+MAX_BEHAVIOURS*BEHAVIOUR_DRAW_YSTEP+4)
#define BEHAVIOUR_DRAW_X 484
#define BEHAVIOUR_DRAW_YBASE (BEHAVIOUR_DRAW_TOP+4)
#define BEHAVIOUR_DRAW_YSTEP 15

#define STATS_DRAW_LEFT 480
#define STATS_DRAW_RIGHT 624
#define STATS_DRAW_Y (BEHAVIOUR_DRAW_BOTTOM+4)

#define GO_DRAW_LEFT 480
#define GO_DRAW_LEFTRIGHT 548
#define GO_DRAW_RIGHTLEFT 556
#define GO_DRAW_RIGHT 624
#define GO_DRAW_TOP (STATS_DRAW_Y+LINE_SPACING*2+4)
#define GO_DRAW_BOTTOM 464

#define OPTIONS_BUTTON_LEFT 400
#define OPTIONS_BUTTON_RIGHT 464
#define OPTIONS_BUTTON_TOP 440
#define OPTIONS_BUTTON_BOTTOM 464
#define OPTIONS_MENU_LEFT 336
#define OPTIONS_MENU_RIGHT 464
#define OPTIONS_SLIDER_LEFT 394
#define OPTIONS_SLIDER_W 46
#define OPTION_H 24

#define OPTION_INSTRUCTIONS 0
#define OPTION_SOUND 1
#define OPTION_MUSIC 2
#define OPTION_SWITCH_MODE 3
#define OPTION_ABOUT 4
#define OPTION_EXIT 5
#define NUM_OPTIONS 6
char optionText[NUM_OPTIONS][64]={
		"Instructions",
		"Sound -----|",
		"Music -----|",
		"Switch mode",
		"About",
		"Exit",
};

#define LINE_SPACING 15

#define CYCLE_TIME 30
#define SMOKE_FRAME_TIME 4
#define SMOKE_NUM_FRAMES 4

struct DraggableBehaviour {
	Behaviour *behaviour;
	int colour;
	int currentX,currentY;
	int destX,destY;
	int codeW,codeH;
	bool lastHover;
};

static int logicTime=0;
static volatile int trueTime=0;

static int lastPingTime=-1;

static volatile bool wantQuit=false;

static DATAFILE *dat;
static BITMAP *scrbuf;
static AL_DUH_PLAYER *adp;

#if MAX_COLOURS!=8
#error Oops!
#endif
static const int numSides[MAX_COLOURS]={0,1,2,3,4,5,8,12};

static int level;
static int levelReached=0;
unsigned char oldMap[MAP_H][MAP_W];
unsigned char newMap[MAP_H][MAP_W];
unsigned char smokeMap[MAP_H][MAP_W];
static DraggableBehaviour allBehaviours[MAX_BEHAVIOURS];
static DraggableBehaviour *assignedBehaviours[MAX_COLOURS];
static DraggableBehaviour *currentDrag;
static int currentDragOption;
static int mx,my;
static int currentDragX,currentDragY;
static int lastMb;
static int mouseRepeatCounter;
static int levelTime;
static int heat;
static int cycles;
static bool success;

static bool showOptions;
static int numOptionsShown;

static bool showInstructions;
static bool showAbout;

static int soundVolume=5,musicVolume=5;
static bool windowed=true;

static void timerProc() {
	trueTime++;
}
END_OF_STATIC_FUNCTION(timerProc);

static void closeProc() {
	wantQuit=true;
}

static void calculateBehaviourPositions() {
	//Put the assigned ones first, next to their colours
	for (int i=1; i<MAX_COLOURS; i++) {
		if (assignedBehaviours[i]!=NULL) {
			assignedBehaviours[i]->destX=COLOUR_DRAW_X+40;
			assignedBehaviours[i]->destY=COLOUR_DRAW_YBASE+i*COLOUR_DRAW_YSTEP+COLOUR_DRAW_YSTEP/2-text_height(font)/2;
		}
	}
	//Put the remaining ones in a section below
	int y=BEHAVIOUR_DRAW_YBASE;
	for (int i=0; i<MAX_BEHAVIOURS; i++) {
		if (allBehaviours[i].behaviour==NULL) break;
		if (allBehaviours[i].colour==-1) {
			allBehaviours[i].destX=BEHAVIOUR_DRAW_X;
			allBehaviours[i].destY=y;
			y+=BEHAVIOUR_DRAW_YSTEP;
		}
	}
}

static void resetLevel() {
	memcpy(oldMap,levels[level].map,sizeof(oldMap));
	memcpy(newMap,levels[level].map,sizeof(newMap));
	heat=0;
	cycles=0;
	levelTime=-1;
	success=false;
}

static void setColour(DraggableBehaviour *b, int c) {
	//Unassign this behaviour its old colour
	if (b->colour>=0) assignedBehaviours[b->colour]=NULL;
	//If any behaviour already has the new colour, give it our old colour
	if (c>=0 && assignedBehaviours[c]!=NULL) setColour(assignedBehaviours[c],b->colour);
	//Assign this behaviour the new colour
	b->colour=c;
	if (b->colour>=0) assignedBehaviours[b->colour]=b;
}

static void startLevel(int _level) {
	level=_level;
	resetLevel();
	for (int i=0; i<MAX_BEHAVIOURS; i++) {
		allBehaviours[i].behaviour=levels[level].behaviours[i];
		allBehaviours[i].colour=-1;
	}
	for (int i=1; i<MAX_COLOURS; i++) assignedBehaviours[i]=NULL;
	//Memory!
	for (int i=0; i<MAX_BEHAVIOURS; i++) {
		int c=levels[level].behaviourColours[i];
		if (c>0 && c<MAX_COLOURS) setColour(&allBehaviours[i],c);
	}
	calculateBehaviourPositions();
	for (int i=0; i<MAX_BEHAVIOURS; i++) {
		if (allBehaviours[i].behaviour==NULL) break;
		allBehaviours[i].currentX=allBehaviours[i].destX;
		allBehaviours[i].currentY=allBehaviours[i].destY;
		allBehaviours[i].codeW=0;
		allBehaviours[i].codeH=0;
		allBehaviours[i].lastHover=false;
	}
	currentDrag=NULL;
	currentDragOption=-1;
}

static void init() {
	scrbuf=create_bitmap(640,480);
	font=(FONT *)dat[THE_FONT].dat;
	startLevel(0);
	int pos=mouse_pos;
	mx=pos>>16; my=pos&65535;
	lastMb=-1;
	mouseRepeatCounter=-1;
	showOptions=false;
	showInstructions=true;
	showAbout=false;
	numOptionsShown=0;
}

static void shutDown() {
	destroy_bitmap(scrbuf);
}

static int getDropTarget(int mx, int my) {
	if (mx>=COLOUR_DRAW_X && my>=COLOUR_DRAW_YBASE) {
		int c=(my-COLOUR_DRAW_YBASE)/COLOUR_DRAW_YSTEP;
		if (c>=1 && c<MAX_COLOURS) return c;
	}
	if (mx>=BEHAVIOUR_DRAW_LEFT && mx<BEHAVIOUR_DRAW_RIGHT && my>=BEHAVIOUR_DRAW_TOP && my<BEHAVIOUR_DRAW_BOTTOM) {
		return -1;
	}
	if (mx>=MAP_DRAW_XBASE && my>=MAP_DRAW_YBASE) {
		int x=(mx-MAP_DRAW_XBASE)/MAP_DRAW_XSTEP;
		int y=(my-MAP_DRAW_YBASE)/MAP_DRAW_YSTEP;
		if (x<MAP_W && y<MAP_H && newMap[y][x]!=0) return newMap[y][x];
	}
	return -2;
}

static void rect2(BITMAP *bmp, int x1, int y1, int x2, int y2, int c) {
	hline(bmp,x1-2,y1,x2+3,c);
	hline(bmp,x1-2,y2,x2+1,c);
	vline(bmp,x1,y1-2,y2+1,c);
	vline(bmp,x2,y1-3,y2+2,c);
}

static void rectfill2(BITMAP *bmp, int x1, int y1, int x2, int y2, int c) {
	drawing_mode(DRAW_MODE_TRANS,NULL,0,0);
	set_trans_blender(0,0,0,216);
	rectfill(bmp,x1,y1,x2,y2,c);
	solid_mode();
}

static void drawButton(int left, int right, int top, int bottom, const char *text, int customFillHighlight, int lightColour, bool enabled, int optionNumber=-2) {
	int fillColour,textColour;
	if (!enabled) {
		fillColour=buttonFillColour; textColour=buttonFillHighlight;
	}
	else if (currentDragOption==optionNumber || (currentDrag==NULL && currentDragOption==-1 && mx>=left && mx<right && my>=top && my<bottom)) {
		fillColour=customFillHighlight; textColour=white;
	}
	else {
		fillColour=buttonFillColour; textColour=lightColour;
	}
	rect2(scrbuf,left,top,right-1,bottom-1,black);
	rectfill2(scrbuf,left+1,top+1,right-2,bottom-2,fillColour);
	textout_centre_ex(scrbuf,font,text,(left+right)/2,(top+bottom)/2-text_height(font)/2,textColour,-1);
}

static void drawInstructions() {
	rect2(scrbuf,32,16,447,431,black);
	rectfill2(scrbuf,33,17,446,430,white);
	int x=240,y=22;
	textout_centre_ex(scrbuf,font,"Elemental Sun",x,y,darkYellow,-1); y+=LINE_SPACING;
	y+=LINE_SPACING;
	textout_centre_ex(scrbuf,font,"Welcome to Your eternal life as a Sun God.",x,y,black,-1); y+=LINE_SPACING;
	y+=LINE_SPACING;
	textout_centre_ex(scrbuf,font,"The Sun contains various chemical elements.",x,y,black,-1); y+=LINE_SPACING;
	textout_centre_ex(scrbuf,font,"Your job is to define the properties of each element.",x,y,black,-1); y+=LINE_SPACING;
	textout_centre_ex(scrbuf,font,"Do this by dragging the available functions on to them.",x,y,black,-1); y+=LINE_SPACING;
	y+=LINE_SPACING;
	textout_centre_ex(scrbuf,font,"If You are successful, the sun will generate heat",x,y,black,-1); y+=LINE_SPACING;
	textout_centre_ex(scrbuf,font,"and provide life for everyone on Earth.",x,y,black,-1); y+=LINE_SPACING;
	y+=LINE_SPACING;
	textout_centre_ex(scrbuf,font,"The sun operates in cycles. During each cycle,",x,y,black,-1); y+=LINE_SPACING;
	textout_centre_ex(scrbuf,font,"all the functions are run on all the elements.",x,y,black,-1); y+=LINE_SPACING;
	textout_centre_ex(scrbuf,font,"Then each element emits heat, and the heat is",x,y,black,-1); y+=LINE_SPACING;
	textout_centre_ex(scrbuf,font,"collected and added to Your total.",x,y,black,-1); y+=LINE_SPACING;
	y+=LINE_SPACING;
	textout_centre_ex(scrbuf,font,"An element's value in heat is equal to",x,y,black,-1); y+=LINE_SPACING;
	textout_centre_ex(scrbuf,font,"the number of sides its drawing has.",x,y,black,-1); y+=LINE_SPACING;
	y+=LINE_SPACING;
	textout_centre_ex(scrbuf,font,"If two functions write to the same square,",x,y,black,-1); y+=LINE_SPACING;
	textout_centre_ex(scrbuf,font,"the square will be occupied by the less valuable element.",x,y,black,-1); y+=LINE_SPACING;
	y+=LINE_SPACING;
	textout_centre_ex(scrbuf,font,"All elements are unstable until You decree otherwise.",x,y,black,-1); y+=LINE_SPACING;
	textout_centre_ex(scrbuf,font,"If an element is not refreshed, it decays to nothing.",x,y,black,-1); y+=LINE_SPACING;
	y+=LINE_SPACING;
	textout_centre_ex(scrbuf,font,"Best of luck with this valuable work.",x,y,black,-1); y+=LINE_SPACING;
	textout_centre_ex(scrbuf,font,"We are certain trillions of life-forms are in good hands.",x,y,black,-1); y+=LINE_SPACING;
}

static void drawAbout() {
	rect2(scrbuf,32,16,447,431,black);
	rectfill2(scrbuf,33,17,446,430,white);
	int x=240,y=22;
	textout_centre_ex(scrbuf,font,"Elemental Sun",x,y,darkYellow,-1); y+=LINE_SPACING;
	y+=LINE_SPACING;
	textout_centre_ex(scrbuf,font,"Copyright (C) 2010 Ben 'ent-\"Bruce `Wants a cool name",x,y,black,-1); y+=LINE_SPACING;
	textout_centre_ex(scrbuf,font,"like Chris Barry or Steve Terry` Perry\"-heh*' Davis",x,y,black,-1); y+=LINE_SPACING;
	y+=LINE_SPACING;
	textout_centre_ex(scrbuf,font,"*The 'th' is hardly pronounced anyway.",x,y,black,-1); y+=LINE_SPACING;
	y+=LINE_SPACING;
	textout_centre_ex(scrbuf,font,"Written in 59 hours for TINS 2010.",x,y,black,-1); y+=LINE_SPACING;
	textout_centre_ex(scrbuf,font,"~~ tins.amarillion.org/2010 ~~",x,y,black,-1); y+=LINE_SPACING;
	textout_centre_ex(scrbuf,font,"It's a 72-hour game programming competition.",x,y,black,-1); y+=LINE_SPACING;
	textout_centre_ex(scrbuf,font,"I spent the other 14 hours sleeping.",x,y,black,-1); y+=LINE_SPACING;
	textout_centre_ex(scrbuf,font,"That's not much sleep. I hope I didn't make an mistakes.",x,y,black,-1); y+=LINE_SPACING;
	y+=LINE_SPACING;
	textout_centre_ex(scrbuf,font,"Please feel free to distribute unmodified",x,y,black,-1); y+=LINE_SPACING;
	textout_centre_ex(scrbuf,font,"copies of this game free of charge.",x,y,black,-1); y+=LINE_SPACING;
	textout_centre_ex(scrbuf,font,"Make sure you include the source code.",x,y,black,-1); y+=LINE_SPACING;
	y+=LINE_SPACING;
	textout_centre_ex(scrbuf,font,"Developed using Allegro 4.4.0.1,",x,y,black,-1); y+=LINE_SPACING;
	textout_centre_ex(scrbuf,font,"JPGAlleg 2.6 and DUMB v0.9.3.",x,y,black,-1); y+=LINE_SPACING;
	y+=LINE_SPACING;
	textout_centre_ex(scrbuf,font,"If you look closely, this game features many sheep.",x,y,black,-1); y+=LINE_SPACING;
	textout_centre_ex(scrbuf,font,"They can most clearly be seen in the final line",x,y,black,-1); y+=LINE_SPACING;
	textout_centre_ex(scrbuf,font,"of the instructions.",x,y,black,-1); y+=LINE_SPACING;
	y+=LINE_SPACING;
	textout_centre_ex(scrbuf,font,"Special thanks to Amarillion for organising",x,y,black,-1); y+=LINE_SPACING;
	textout_centre_ex(scrbuf,font,"the competition, and for allowing me to compete",x,y,black,-1); y+=LINE_SPACING;
	textout_centre_ex(scrbuf,font,"even after my entry 'Aliens on Fire' last time.",x,y,black,-1); y+=LINE_SPACING;
}

static void drawStatusMessage(const char *message) {
	rect2(scrbuf,160,MAP_DRAW_YBASE+MAP_H*MAP_DRAW_YSTEP+4,319,MAP_DRAW_YBASE+MAP_H*MAP_DRAW_YSTEP+27,black);
	rectfill2(scrbuf,161,MAP_DRAW_YBASE+MAP_H*MAP_DRAW_YSTEP+5,318,MAP_DRAW_YBASE+MAP_H*MAP_DRAW_YSTEP+26,white);
	textout_centre_ex(scrbuf,font,message,240,MAP_DRAW_YBASE+MAP_H*MAP_DRAW_YSTEP+16-text_height(font)/2,red,-1);
}

static void draw() {
	blit((BITMAP *)dat[BACKGROUND].dat,scrbuf,0,0,0,0,640,480);
	int dropTarget=-2;
	if (currentDrag!=NULL && currentDragOption==-1) dropTarget=getDropTarget(mx,my);
	//Draw the map
	for (int y=0; y<MAP_H; y++) {
		for (int x=0; x<MAP_W; x++) {
			int xs=MAP_DRAW_XBASE+x*MAP_DRAW_XSTEP;
			int ys=MAP_DRAW_YBASE+y*MAP_DRAW_YSTEP;
			int c=newMap[y][x];
			if (c>=1) {
				BITMAP *sprite=(BITMAP *)dat[SPRITES_000-1+c].dat;
				if (c==dropTarget) {
					set_trans_blender(0,0,0,0);
					draw_lit_sprite(scrbuf,sprite,xs,ys,48);
				}
				else draw_sprite(scrbuf,sprite,xs,ys);
			}
			if (levelTime>=1 && smokeMap[y][x]!=0) {
				int frame=((levelTime-1)%CYCLE_TIME)/SMOKE_FRAME_TIME;
				if (frame<SMOKE_NUM_FRAMES) {
					BITMAP *smoke=(BITMAP *)dat[SMOKE_000+frame].dat;
					draw_sprite(scrbuf,smoke,xs,ys);
				}
			}
		}
	}
	//Draw the colours down the side, for behaviours to be assigned to
	for (int c=1; c<MAX_COLOURS; c++) {
		int xs=COLOUR_DRAW_X;
		int ys=COLOUR_DRAW_YBASE+c*COLOUR_DRAW_YSTEP;
		BITMAP *sprite=(BITMAP *)dat[SPRITES_000-1+c].dat;
		if (c==dropTarget) {
			set_trans_blender(0,0,0,0);
			draw_lit_sprite(scrbuf,sprite,xs,ys,48);
		}
		else draw_sprite(scrbuf,sprite,xs,ys);
	}
	//Draw the box for unassigned behaviours
	rect2(scrbuf,BEHAVIOUR_DRAW_LEFT,BEHAVIOUR_DRAW_TOP,BEHAVIOUR_DRAW_RIGHT-1,BEHAVIOUR_DRAW_BOTTOM-1,black);
	//Draw the stats
	if (levelTime<0) {
		textprintf_centre_ex(scrbuf,font,(STATS_DRAW_LEFT+STATS_DRAW_RIGHT)/2,STATS_DRAW_Y,black,-1,"Get %d heat",levels[level].targetHeat);
		textprintf_centre_ex(scrbuf,font,(STATS_DRAW_LEFT+STATS_DRAW_RIGHT)/2,STATS_DRAW_Y+LINE_SPACING,black,-1,"in %d %s!",levels[level].targetCycles,levels[level].targetCycles==1?"cycle":"cycles");
	}
	else {
		textout_ex(scrbuf,font,"Heat:",STATS_DRAW_LEFT,STATS_DRAW_Y,black,-1);
		textprintf_right_ex(scrbuf,font,STATS_DRAW_RIGHT,STATS_DRAW_Y,black,-1,"%d/%d",heat,levels[level].targetHeat);
		textout_ex(scrbuf,font,"Cycle:",STATS_DRAW_LEFT,STATS_DRAW_Y+LINE_SPACING,black,-1);
		textprintf_right_ex(scrbuf,font,STATS_DRAW_RIGHT,STATS_DRAW_Y+LINE_SPACING,black,-1,"%d/%d",cycles,levels[level].targetCycles);
	}
	//Draw the level indicator and buttons
	drawButton(LEVEL_LEFT,LEVEL_LEFT+LEVEL_BUTTON_W,LEVEL_TOP,LEVEL_BOTTOM,"<",buttonFillHighlight,white,level>0);
	drawButton(LEVEL_RIGHT-LEVEL_BUTTON_W,LEVEL_RIGHT,LEVEL_TOP,LEVEL_BOTTOM,">",buttonFillHighlight,white,level<levelReached);
	textprintf_centre_ex(scrbuf,font,(LEVEL_LEFT+LEVEL_RIGHT)/2,(LEVEL_TOP+LEVEL_BOTTOM)/2-text_height(font)/2,black,-1,"Level %d/%d",level+1,numLevels);
	//Draw the 'go' or 'stop' button
	if (showInstructions || showAbout) drawButton(GO_DRAW_LEFT,GO_DRAW_RIGHT,GO_DRAW_TOP,GO_DRAW_BOTTOM,"OK",buttonFillHighlight,white,true);
	else if (levelTime<0) drawButton(GO_DRAW_LEFT,GO_DRAW_RIGHT,GO_DRAW_TOP,GO_DRAW_BOTTOM,"GO!",darkGreen,lightGreen,true);
	else if (success) {
		if (level<levelReached) {
			drawButton(GO_DRAW_LEFT,GO_DRAW_LEFTRIGHT,GO_DRAW_TOP,GO_DRAW_BOTTOM,"Redo",darkRed,lightRed,true);
			drawButton(GO_DRAW_RIGHTLEFT,GO_DRAW_RIGHT,GO_DRAW_TOP,GO_DRAW_BOTTOM,"Next",darkGreen,lightGreen,true);
		}
		else drawButton(GO_DRAW_LEFT,GO_DRAW_RIGHT,GO_DRAW_TOP,GO_DRAW_BOTTOM,"Redo",darkRed,lightRed,true);
	}
	else if (cycles>=levels[level].targetCycles) {
		drawButton(GO_DRAW_LEFT,GO_DRAW_RIGHT,GO_DRAW_TOP,GO_DRAW_BOTTOM,"Retry",darkRed,lightRed,true);
	}
	else drawButton(GO_DRAW_LEFT,GO_DRAW_RIGHT,GO_DRAW_TOP,GO_DRAW_BOTTOM,"Stop",darkRed,lightRed,true);
	//Draw the options button
	drawButton(OPTIONS_BUTTON_LEFT,OPTIONS_BUTTON_RIGHT,OPTIONS_BUTTON_TOP,OPTIONS_BUTTON_BOTTOM,"Options",buttonFillHighlight,white,true);
	//Draw the status message
	if (success) {
		if (level<levelReached) drawStatusMessage("TARGET ACHIEVED");
		else drawStatusMessage("GAME COMPLETE");
	}
	else if (cycles>=levels[level].targetCycles) drawStatusMessage("TARGET MISSED");
	//Draw the available behaviours
	for (int i=0; i<MAX_BEHAVIOURS; i++) {
		Behaviour *b=allBehaviours[i].behaviour;
		if (b!=NULL) {
			int x=allBehaviours[i].currentX, y=allBehaviours[i].currentY;
			textout_ex(scrbuf,font,b->name,x,y,black,-1);
		}
	}
	//If options are enabled, draw them
	for (int i=0; i<=5; i++) optionText[OPTION_SOUND][6+i]=(soundVolume==i?'|':'-');
	for (int i=0; i<=5; i++) optionText[OPTION_MUSIC][6+i]=(musicVolume==i?'|':'-');
	strcpy(optionText[OPTION_SWITCH_MODE],windowed?"Go full screen":"Go windowed");
	int y=OPTIONS_BUTTON_TOP-OPTION_H*numOptionsShown;
	for (int i=NUM_OPTIONS-numOptionsShown; i<NUM_OPTIONS; i++) {
		drawButton(OPTIONS_MENU_LEFT,OPTIONS_MENU_RIGHT,y,y+OPTION_H,optionText[i],buttonFillHighlight,white,true,i);
		y+=OPTION_H;
	}
	//If other screens are enabled, draw them
	if (showInstructions) drawInstructions();
	if (showAbout) drawAbout();
	//Draw the code popups
	for (int i=0; i<MAX_BEHAVIOURS; i++) {
		Behaviour *b=allBehaviours[i].behaviour;
		if (b!=NULL) {
			int codeW=allBehaviours[i].codeW, codeH=allBehaviours[i].codeH;
			if (codeW>0 && codeH>0) {
				//Calculate where to draw the code
				int xcode=allBehaviours[i].currentX-8-codeW, ycode=allBehaviours[i].currentY;
				if (ycode+codeH>476) ycode=476-codeH;
				rect2(scrbuf,xcode,ycode,xcode+codeW-1,ycode+codeH-1,black);
				if (codeW>2 && codeH>2) {
					rectfill2(scrbuf,xcode+1,ycode+1,xcode+codeW-2,ycode+codeH-2,darkGrey);
					set_clip_rect(scrbuf,xcode+4,ycode+4,xcode+codeW-5,ycode+codeH-5);
					for (int l=0; l<b->numSourceLines; l++) {
						textout_ex(scrbuf,font,b->sourceLines[l],xcode+4,ycode+4+l*LINE_SPACING,green,-1);
					}
					set_clip_rect(scrbuf,0,0,scrbuf->w-1,scrbuf->h-1);
				}
			}
		}
	}
	//Draw the mouse pointer
	draw_sprite(scrbuf,(BITMAP *)dat[POINTER].dat,mx,my);
	acquire_screen();
	blit(scrbuf,screen,0,0,0,0,640,480);
	release_screen();
}

static void updateLevelMemory() {
	//Memory!
	for (int i=0; i<MAX_BEHAVIOURS; i++) {
		levels[level].behaviourColours[i]=allBehaviours[i].colour;
	}
}

static void playPing() {
	if (logicTime>lastPingTime) {
		play_sample((SAMPLE *)dat[SFX_PING].dat,64*soundVolume/5,128,1000,false);
		lastPingTime=logicTime;
	}
}

static void switchMode(bool alreadyReverting) {
	al_pause_duh(adp);
	windowed=!windowed;
	int gfxMode=(windowed?GFX_AUTODETECT_WINDOWED:GFX_AUTODETECT_FULLSCREEN);
	if (set_gfx_mode(gfxMode,640,480,0,0)!=0) {
		if (alreadyReverting) {
			set_gfx_mode(GFX_TEXT,0,0,0,0);
			allegro_message("Unable to revert to previous mode (windowed/full screen)");
			exit(EXIT_FAILURE);
		}
		else switchMode(true);
	}
	//Try both modes. Hopefully bloody Windows will succeed for at least one of them!
	set_display_switch_mode(SWITCH_BACKGROUND);
	set_display_switch_mode(SWITCH_BACKAMNESIA);
	logicTime=trueTime;
	al_resume_duh(adp);
}

static void update() {
	int mb=mouse_b;
	int pos=mouse_pos;
	mx=pos>>16; my=pos&65535;
	bool showOkButton=(showInstructions || showAbout);
	if (showOkButton && mb!=0 && lastMb==0) {playPing(); showInstructions=showAbout=false;}
	calculateBehaviourPositions();
	for (int i=0; i<MAX_BEHAVIOURS; i++) {
		DraggableBehaviour *b=&allBehaviours[i];
		if (b->behaviour==NULL) break;
		if (mb!=0 && currentDrag==NULL && currentDragOption==-1 && levelTime<0) {
			if (mx>=b->currentX && mx<b->currentX+text_length(font,b->behaviour->name) && my>=b->currentY && my<b->currentY+text_height(font)) {
				playPing();
				currentDrag=b;
				currentDragX=mx-b->currentX;
				currentDragY=my-b->currentY;
			}
		}
		if (currentDrag==b) {
			if (mb!=0) {
				b->currentX=mx-currentDragX;
				b->currentY=my-currentDragY;
			}
			else {
				//Work out where it has been dropped, and set or clear its colour accordingly
				int c=getDropTarget(mx,my);
				if (c!=-2) {playPing(); setColour(b,c); updateLevelMemory();}
				currentDrag=NULL;
				calculateBehaviourPositions();
			}
		}
		if (currentDrag!=b) {
			b->currentX=b->destX+(b->currentX-b->destX)*3/4;
			b->currentY=b->destY+(b->currentY-b->destY)*3/4;
		}
		//Is the mouse over this behaviour?
		int x=b->currentX, y=b->currentY;
		int destW=0,destH=0;
		bool hover=false;
		if ((currentDrag==NULL && currentDragOption==-1) || currentDrag==b) {
			if (mx>=x && mx<x+text_length(font,b->behaviour->name) && my>=y && my<y+text_height(font)) {
				//Get the longest line length in the code
				int maxLen=0;
				for (int l=0; l<b->behaviour->numSourceLines; l++) {
					int len=text_length(font,b->behaviour->sourceLines[l]);
					if (len>maxLen) maxLen=len;
				}
				//Calculate the destination code view size
				destW=maxLen+8;
				destH=b->behaviour->numSourceLines*LINE_SPACING+8;
				hover=true;
			}
		}
		if (hover && !b->lastHover) {
			int pan=32+(b->currentX-8-destW/2)*192/640;
			if (pan<0) pan=0;
			play_sample((SAMPLE *)dat[SFX_WSHH].dat,32*soundVolume/5,pan,1000,false);
		}
		if (!hover && b->lastHover) {
			int pan=32+(b->currentX-8-b->codeW/2)*192/640;
			if (pan<0) pan=0;
			play_sample((SAMPLE *)dat[SFX_SHHW].dat,32*soundVolume/5,pan,1000,false);
		}
		b->lastHover=hover;
		//Scale the code window
		b->codeW=destW+(b->codeW-destW)*3/4;
		b->codeH=destH+(b->codeH-destH)*3/4;
	}
	if (!showOkButton && mb!=0 && lastMb==0 && mx>=GO_DRAW_LEFT && mx<GO_DRAW_RIGHT && my>=GO_DRAW_TOP && my<GO_DRAW_BOTTOM) {
		if (levelTime<0) {
			//GO!
			playPing();
			levelTime=0;
		}
		else if (success && level<levelReached) {
			//Redo
			if (mx<GO_DRAW_LEFTRIGHT) {playPing(); resetLevel();}
			//Next
			if (mx>=GO_DRAW_RIGHTLEFT) {playPing(); startLevel(level+1);}
		}
		else {
			//Stop/Retry
			playPing();
			resetLevel();
		}
	}
	if (mb!=0 && lastMb==0 && mx>=OPTIONS_BUTTON_LEFT && mx<OPTIONS_BUTTON_RIGHT && my>=OPTIONS_BUTTON_TOP && my<OPTIONS_BUTTON_BOTTOM) {
		playPing();
		showOptions=!showOptions;
		lastMb=mb;
	}
	if (showOptions && numOptionsShown<NUM_OPTIONS) numOptionsShown++;
	if (!showOptions && numOptionsShown>0) numOptionsShown--;
	if (showOptions) {
		bool justClicked=false;
		if (mx>=OPTIONS_MENU_LEFT && mx<OPTIONS_MENU_RIGHT && my>=OPTIONS_BUTTON_TOP-OPTION_H*NUM_OPTIONS && my<OPTIONS_BUTTON_TOP) {
			int option=(my-(OPTIONS_BUTTON_TOP-OPTION_H*NUM_OPTIONS))/OPTION_H;
			if (option==OPTION_INSTRUCTIONS && mb!=0 && lastMb==0) {playPing(); showOptions=false; showInstructions=true;}
			if ((option==OPTION_SOUND || option==OPTION_MUSIC) && mb!=0 && currentDragOption==-1) {currentDragOption=option; justClicked=true;}
			if (option==OPTION_SWITCH_MODE && mb!=0 && lastMb==0) {playPing(); switchMode(false);}
			if (option==OPTION_ABOUT && mb!=0 && lastMb==0) {playPing(); showOptions=false; showAbout=true;}
			if (option==OPTION_EXIT && mb!=0 && lastMb==0) {playPing(); showOptions=false; wantQuit=true;}
		}
		else if (mb!=0 && lastMb==0) {playPing(); showOptions=false;}
		int sliderPos=(mx-OPTIONS_SLIDER_LEFT)*5/OPTIONS_SLIDER_W;
		if (sliderPos<0) sliderPos=0;
		if (sliderPos>5) sliderPos=5;
		if (currentDragOption==OPTION_SOUND && sliderPos!=soundVolume) {soundVolume=sliderPos; playPing();}
		if (currentDragOption==OPTION_MUSIC) musicVolume=sliderPos;
		if (justClicked) playPing();
	}
	if (mb==0) currentDragOption=-1;
	if (mb!=0 && (lastMb==0 || mouseRepeatCounter==0)) {
		if (mx>=LEVEL_LEFT && mx<LEVEL_LEFT+LEVEL_BUTTON_W && my>=LEVEL_TOP && my<LEVEL_BOTTOM) {
			if (level>0) {playPing(); startLevel(level-1);}
			if (mouseRepeatCounter<0) mouseRepeatCounter=MOUSE_REPEAT_INITIAL;
			else mouseRepeatCounter=MOUSE_REPEAT_SUBSEQUENT;
		}
		else if (mx>=LEVEL_RIGHT-LEVEL_BUTTON_W && mx<LEVEL_RIGHT && my>=LEVEL_TOP && my<LEVEL_BOTTOM) {
			if (level<levelReached) {playPing(); startLevel(level+1);}
			if (mouseRepeatCounter<0) mouseRepeatCounter=MOUSE_REPEAT_INITIAL;
			else mouseRepeatCounter=MOUSE_REPEAT_SUBSEQUENT;
		}
	}
	if (mouseRepeatCounter>0) mouseRepeatCounter--;
	if (mb==0 || currentDrag!=NULL || currentDragOption!=-1) mouseRepeatCounter=-1;
	lastMb=mb;
	if (levelTime>=0) {
		if (levelTime%CYCLE_TIME==0) {
			memcpy(oldMap,newMap,sizeof(newMap));
			memset(newMap,255,sizeof(newMap));
			memset(smokeMap,0,sizeof(smokeMap));
			for (int y=0; y<MAP_H; y++) {
				for (int x=0; x<MAP_W; x++) {
					DraggableBehaviour *b=assignedBehaviours[oldMap[y][x]];
					if (b!=NULL) b->behaviour->animate(x,y);
				}
			}
			int smokeCount=0, smokePan=0;
			for (int y=0; y<MAP_H; y++) {
				for (int x=0; x<MAP_W; x++) {
					if (newMap[y][x]==255) {
						newMap[y][x]=0;
						if (oldMap[y][x]!=0) smokeMap[y][x]=1;
					}
					if (cycles<levels[level].targetCycles) heat+=numSides[newMap[y][x]];
					smokeCount+=smokeMap[y][x];
					smokePan+=smokeMap[y][x]*x;
				}
			}
			if (smokeCount>0) {
				smokePan=32+smokePan*192/((MAP_W-1)*smokeCount);
				int smokeVol=(int)(sqrt(smokeCount)*16);
				if (smokeVol>64) smokeVol=64;
				play_sample((SAMPLE *)dat[SFX_POW].dat,smokeVol*soundVolume/5,smokePan,1000,false);
			}
			if (cycles<levels[level].targetCycles) {
				if (smokeCount==0) playPing();
				cycles++;
			}
			if (!success && cycles<=levels[level].targetCycles && heat>=levels[level].targetHeat) {
				playPing();
				success=true;
				if (levelReached==level && levelReached<numLevels-1) levelReached++;
			}
		}
		levelTime++;
	}
}

static void handleKey(int k) {
	if (k>>8==KEY_ESC) {
		//'Esc' closes the currently open popup if any, or else stops the level
		if (showInstructions) {playPing(); showInstructions=false;}
		else if (showAbout) {playPing(); showAbout=false;}
		else if (showOptions) {playPing(); showOptions=false;}
		else if (levelTime>=0) {playPing(); resetLevel();}
		//When in full screen, 'Esc' is also an emergency 'go windowed' key
		else if (!windowed) switchMode(false);
	}
}

int main() {
	LOCK_VARIABLE(trueTime);
	LOCK_FUNCTION(timerProc);
	set_config_file("elemsun.ini");
	allegro_init();
	set_window_title("Elemental Sun");
	set_close_button_callback(closeProc);
	windowed=get_config_int("elemsun","windowed",windowed);
	soundVolume=get_config_int("elemsun","sound",soundVolume);
	musicVolume=get_config_int("elemsun","music",musicVolume);
	levelReached=get_config_int("elemsun","level",levelReached);
	soundVolume=MID(0,soundVolume,5);
	musicVolume=MID(0,musicVolume,5);
	levelReached=MID(0,levelReached,numLevels-1);
	jpgalleg_init();
	atexit(&dumb_exit);
	install_timer();
	install_keyboard();
	install_mouse();
	reserve_voices(64,-1);
	set_volume_per_voice(0);
	install_sound(DIGI_AUTODETECT,MIDI_NONE,NULL);
	dumb_register_dat_it_quick(DUMB_DAT_IT);
	readSource();
	int gfxMode=(windowed?GFX_AUTODETECT_WINDOWED:GFX_AUTODETECT_FULLSCREEN);
	int desktopDepth=desktop_color_depth();
	if (desktopDepth>8) set_color_depth(desktopDepth);
	if (desktopDepth<=8 || set_gfx_mode(gfxMode,640,480,0,0)!=0) {
		set_color_depth(32);
		if (set_gfx_mode(gfxMode,640,480,0,0)!=0) {
			set_color_depth(24);
			if (set_gfx_mode(gfxMode,640,480,0,0)!=0) {
				set_color_depth(16);
				if (set_gfx_mode(gfxMode,640,480,0,0)!=0) {
					set_color_depth(15);
					if (set_gfx_mode(gfxMode,640,480,0,0)!=0) {
						allegro_message("Unable to set graphics mode: %s",allegro_error);
						return EXIT_FAILURE;
					}
				}
			}
		}
	}
	//Try both modes. Hopefully bloody Windows will succeed for at least one of them!
	set_display_switch_mode(SWITCH_BACKGROUND);
	set_display_switch_mode(SWITCH_BACKAMNESIA);
	//I wanted to load the datafile first and then use fixup_datafile(),
	//but alas, it doesn't work on JPEGs!
	dat=load_datafile("elemsun.dat");
	if (dat==NULL) {
		set_gfx_mode(GFX_TEXT,0,0,0,0);
		allegro_message("Unable to load elemsun.dat");
		return EXIT_FAILURE;
	}
	setColours();
	adp=al_start_duh((DUH *)dat[THE_MUSIC].dat,2,0,musicVolume/5.0,4096,44100);
	init();
	install_int_ex(timerProc,BPS_TO_TIMER(50));
	while (!wantQuit) {
		al_duh_set_volume(adp,musicVolume/5.0);
		al_poll_duh(adp);
		draw();
		while (!wantQuit && logicTime>=trueTime) rest(1);
		while (!wantQuit && keypressed()) handleKey(readkey());
		int numUpdates=0;
		while (!wantQuit && logicTime<trueTime) {
			update();
			logicTime++;
			numUpdates++;
			if (numUpdates==10) {logicTime=trueTime; break;}
		}
	}
	al_stop_duh(adp);
	shutDown();
	set_gfx_mode(GFX_TEXT,0,0,0,0);
	set_config_int("elemsun","windowed",windowed);
	set_config_int("elemsun","sound",soundVolume);
	set_config_int("elemsun","music",musicVolume);
	set_config_int("elemsun","level",levelReached);
	return EXIT_SUCCESS;
}
END_OF_MAIN();
