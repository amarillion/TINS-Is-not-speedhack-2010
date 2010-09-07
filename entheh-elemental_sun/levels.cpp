#include "elemsun.h"

static unsigned char get(int x, int y) {
	if (x>=0 && x<MAP_W && y>=0 && y<MAP_H) return oldMap[y][x];
	else return 0;
}

static void put(int x, int y, unsigned char c) {
	if (c>=MAX_COLOURS) c=0;
	if (x>=0 && x<MAP_W && y>=0 && y<MAP_H) {
		if (newMap[y][x]>c) newMap[y][x]=c;
		smokeMap[y][x]=1;
	}
}

static void inert(int x, int y) {
	put(x,y,get(x,y));
}

static void explode(int x, int y) {
	put(x-1,y,get(x,y)+1);
	put(x+1,y,get(x,y)+1);
	put(x,y-1,get(x,y)+1);
	put(x,y+1,get(x,y)+1);
	put(x,y,0);
}

static void clock(int x, int y) {
	int c=get(x,y);
	int l=get(x-1,y)+get(x-1,y+1)-2*get(x,y+1);
	int b=get(x,y+1)+get(x+1,y+1)-2*get(x+1,y);
	int r=get(x+1,y)+get(x+1,y-1)-2*get(x,y-1);
	int t=get(x,y-1)+get(x-1,y-1)-2*get(x-1,y);
	if (l>b && l>r && l>t) put(x,y+1,c);
	if (b>r && b>t && b>l) put(x+1,y,c);
	if (r>t && r>l && r>b) put(x,y-1,c);
	if (t>l && t>b && t>r) put(x-1,y,c);
}

static void anticlock(int x, int y) {
	int c=get(x,y);
	int l=get(x-1,y)+get(x-1,y-1)-2*get(x,y-1);
	int t=get(x,y-1)+get(x+1,y-1)-2*get(x+1,y);
	int r=get(x+1,y)+get(x+1,y+1)-2*get(x,y+1);
	int b=get(x,y+1)+get(x-1,y+1)-2*get(x-1,y);
	if (l>t && l>r && l>b) put(x,y-1,c);
	if (t>r && t>b && t>l) put(x+1,y,c);
	if (r>b && r>l && r>t) put(x,y+1,c);
	if (b>l && b>t && b>r) put(x-1,y,c);
}

static void emit(int x, int y) {
	put(x,y,get(x,y));
	if (get(x-1,y)==0) put(x-1,y,1);
	if (get(x+1,y)==0) put(x+1,y,1);
	if (get(x,y-1)==0) put(x,y-1,1);
	if (get(x,y+1)==0) put(x,y+1,1);
}

static void disperse(int x, int y) {
	put(x-1,y,1);
	put(x+1,y,1);
	put(x,y-1,1);
	put(x,y+1,1);
	put(x,y,get(x,y)-1);
}

static void decay(int x, int y) {
	put(x-1,y,1);
	put(x+1,y,1);
	put(x,y-1,1);
	put(x,y+1,1);
	put(x,y,0);
}

static void mimic(int x, int y) {
	int c=0;
	if (get(x-1,y)>c) c=get(x-1,y);
	if (get(x+1,y)>c) c=get(x+1,y);
	if (get(x,y-1)>c) c=get(x,y-1);
	if (get(x,y+1)>c) c=get(x,y+1);
	put(x,y,c);
}

static void breed(int x, int y) {
	int neighbours=0;
	if (get(x-1,y)>=get(x,y)) neighbours++;
	if (get(x+1,y)>=get(x,y)) neighbours++;
	if (get(x,y-1)>=get(x,y)) neighbours++;
	if (get(x,y+1)>=get(x,y)) neighbours++;
	if (get(x-1,y-1)>=get(x,y)) neighbours++;
	if (get(x+1,y-1)>=get(x,y)) neighbours++;
	if (get(x-1,y+1)>=get(x,y)) neighbours++;
	if (get(x+1,y+1)>=get(x,y)) neighbours++;
	if (neighbours==3) put(x,y,get(x,y)+1);
	else if (neighbours==2) put(x,y,get(x,y));
	else put(x,y,get(x,y)-1);
}

static void sink(int x, int y) {
	int c=0, d=get(x,y);
	if (get(x-1,y)>c && get(x-1,y)<d) c=get(x-1,y);
	if (get(x+1,y)>c && get(x+1,y)<d) c=get(x+1,y);
	if (get(x,y-1)>c && get(x,y-1)<d) c=get(x,y-1);
	if (get(x,y+1)>c && get(x,y+1)<d) c=get(x,y+1);
	if (c>0) {
		if (get(x-1,y)==c) put(x-1,y,c-1);
		if (get(x+1,y)==c) put(x+1,y,c-1);
		if (get(x,y-1)==c) put(x,y-1,c-1);
		if (get(x,y+1)==c) put(x,y+1,c-1);
		put(x,y,d+1);
	}
	else put(x,y,d-1);
}

static Behaviour _inert={"inert()",inert};
static Behaviour _explode={"explode()",explode};
static Behaviour _clock={"clock()",clock};
static Behaviour _anticlock={"anticlock()",anticlock};
static Behaviour _emit={"emit()",emit};
static Behaviour _disperse={"disperse()",disperse};
static Behaviour _decay={"decay()",decay};
static Behaviour _mimic={"mimic()",mimic};
static Behaviour _breed={"breed()",breed};
static Behaviour _sink={"sink()",sink};

Level levels[]={
		{
				{
						{0,0,1,0,0,0,0,1,0,0},
						{0,0,0,0,1,1,0,0,0,0},
						{1,0,1,1,1,1,1,1,0,1},
						{0,0,1,1,1,1,1,1,0,0},
						{0,1,1,1,1,1,1,1,1,0},
						{0,1,1,1,1,1,1,1,1,0},
						{0,0,1,1,1,1,1,1,0,0},
						{1,0,1,1,1,1,1,1,0,1},
						{0,0,0,0,1,1,0,0,0,0},
						{0,0,1,0,0,0,0,1,0,0},
				},
				{&_inert},
				/*targetHeat*/ 100,
				/*targetCycles*/ 5,
		},
		{
				{
						{0,0,0,0,1,1,0,0,0,0},
						{0,0,0,0,1,1,0,0,0,0},
						{0,0,0,0,1,1,0,0,0,0},
						{0,0,0,0,1,1,0,0,0,0},
						{1,1,1,1,2,2,1,1,1,1},
						{1,1,1,1,2,2,1,1,1,1},
						{0,0,0,0,1,1,0,0,0,0},
						{0,0,0,0,1,1,0,0,0,0},
						{0,0,0,0,1,1,0,0,0,0},
						{0,0,0,0,1,1,0,0,0,0},
				},
				{&_explode,&_inert},
				/*targetHeat*/ 100,
				/*targetCycles*/ 3,
		},
		{
				{
						{0,0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0,0},
						{0,0,0,0,1,2,0,0,0,0},
						{0,0,0,2,2,2,1,0,0,0},
						{0,0,0,1,2,2,2,0,0,0},
						{0,0,0,0,2,1,0,0,0,0},
						{0,0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0,0},
				},
				{&_clock,&_explode,&_inert},
				/*targetHeat*/ 250,
				/*targetCycles*/ 7,
		},
		{
				{
						{0,0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0,0},
						{0,0,1,1,0,0,0,0,0,0},
						{0,1,0,0,1,1,1,0,0,0},
						{0,1,0,3,1,0,0,1,0,0},
						{0,1,0,0,0,0,0,1,0,0},
						{0,0,1,1,2,0,0,1,0,0},
						{0,0,0,0,1,1,1,0,0,0},
						{0,0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0,0},
				},
				{&_anticlock,&_clock,&_inert},
				/*targetHeat*/ 200,
				/*targetCycles*/ 10,
		},
		{
				{
						{0,0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,1,0,0,0,0},
						{0,0,0,0,1,0,0,0,0,0},
						{0,0,1,0,0,0,1,0,0,0},
						{0,0,0,1,0,0,0,1,0,0},
						{0,0,0,0,0,1,0,0,0,0},
						{0,0,0,0,1,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0,0},
				},
				{&_breed,&_explode,&_inert},
				/*targetHeat*/ 250,
				/*targetCycles*/ 5,
		},
		{
				{
						{0,0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0,0},
						{0,0,0,0,1,3,0,0,0,0},
						{0,0,0,3,1,1,1,0,0,0},
						{0,0,0,1,1,1,3,0,0,0},
						{0,0,0,0,3,1,0,0,0,0},
						{0,0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0,0},
				},
				{&_clock,&_explode,&_inert},
				/*targetHeat*/ 450,
				/*targetCycles*/ 7,
		},
		{
				{
						{0,0,0,0,0,0,0,2,0,0},
						{0,0,0,2,0,0,0,0,0,0},
						{0,2,0,0,3,2,0,0,2,0},
						{0,0,0,2,3,3,0,0,0,0},
						{0,0,3,3,3,3,3,2,2,0},
						{2,0,0,3,3,3,3,0,0,0},
						{0,0,2,3,3,3,3,0,0,2},
						{0,0,2,3,2,0,2,2,0,0},
						{2,0,0,0,0,0,0,0,2,0},
						{0,0,0,0,2,0,2,0,0,0},
				},
				{&_breed,&_clock,&_inert},
				/*targetHeat*/ 512,
				/*targetCycles*/ 7,
		},
		{
				{
						{0,0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0,0},
						{0,0,3,0,0,0,0,0,0,0},
						{0,0,2,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,2,0,0},
						{0,0,0,0,0,0,0,3,0,0},
						{0,0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0,0},
				},
				{&_breed,&_inert,&_explode,&_mimic},
				/*targetHeat*/ 1000,
				/*targetCycles*/ 11,
		},
		{
				{
						{0,0,0,0,0,0,0,0,0,0},
						{0,0,0,3,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,3,0},
						{0,0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0,0},
						{0,3,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,3,0,0,0},
						{0,0,0,0,0,0,0,0,0,0},
				},
				{&_breed,&_decay,&_disperse,&_mimic},
				/*targetHeat*/ 1000,
				/*targetCycles*/ 12,
		},
		{
				{
						{0,0,0,2,0,0,0,0,1,1},
						{0,2,1,0,0,0,0,0,0,1},
						{0,0,0,0,0,2,0,0,0,0},
						{0,0,0,0,0,6,2,0,0,1},
						{0,1,0,0,0,0,0,0,1,1},
						{1,0,1,0,0,0,0,0,0,0},
						{1,0,1,0,0,0,0,0,0,0},
						{0,1,0,0,1,1,0,0,0,0},
						{0,0,0,1,0,0,1,0,0,0},
						{0,0,0,0,1,1,0,0,0,0},
				},
				{&_anticlock,&_breed,&_clock},
				/*targetHeat*/ 480,
				/*targetCycles*/ 24,
		},
		{
				{
						{0,0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,3,0,0},
						{0,0,0,0,0,0,0,2,0,0},
						{0,0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0,0},
						{0,0,2,0,0,0,0,0,0,0},
						{0,0,3,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0,0},
				},
				{&_breed,&_emit,&_explode,&_mimic},
				/*targetHeat*/ 1500,
				/*targetCycles*/ 9,
		},
		{
				{
						{0,1,1,0,0,0,0,0,0,0},
						{1,0,1,0,0,0,0,0,0,0},
						{1,1,0,0,0,0,0,0,0,0},
						{1,0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0,0},
				},
				{&_breed,&_breed,&_explode,&_explode,&_inert,&_sink,&_sink},
				/*targetHeat*/ 10000,
				/*targetCycles*/ 50,
		},
};

int numLevels=sizeof(levels)/sizeof(Level);
