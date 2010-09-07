#ifndef T3F_PARTICLE_H
#define T3F_PARTICLE_H

#include <allegro5/allegro5.h>
#include "3d.h"
#include "animation.h"

typedef struct
{
	
	ANIMATION * animation;
	
	float x, y, z;
	float vx, vy, vz;
	int life;
	
} T3F_PARTICLE;

#endif
