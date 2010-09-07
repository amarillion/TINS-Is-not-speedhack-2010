#ifndef ELEMSUN_H
#define ELEMSUN_H

#include <stdlib.h>
#include <math.h>
#include <allegro.h>

#define RND(n) ((int)(rand()*((n)/(RAND_MAX+1.0))))

#define MAX_SOURCE_LINES 40

struct Behaviour {
	const char *name;
	void (*animate)(int x, int y);
	int numSourceLines;
	char *sourceLines[MAX_SOURCE_LINES];
};

#define MAP_W 10
#define MAP_H 10
#define MAX_COLOURS 8
#define MAX_BEHAVIOURS 7

struct Level {
	unsigned char map[MAP_H][MAP_W];
	Behaviour *behaviours[MAX_BEHAVIOURS];
	int targetHeat,targetCycles;
	//Memory!
	int behaviourColours[MAX_BEHAVIOURS];
};

extern Level levels[];
extern int numLevels;

extern unsigned char oldMap[MAP_H][MAP_W];
extern unsigned char newMap[MAP_H][MAP_W];
extern unsigned char smokeMap[MAP_H][MAP_W];

void readSource();

#endif /* ELEMSUN_H */
