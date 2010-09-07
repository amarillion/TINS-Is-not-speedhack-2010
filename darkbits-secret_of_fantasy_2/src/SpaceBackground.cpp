#include "Precompiled.hpp"
#include "SpaceBackground.hpp"

SpaceBackground::SpaceBackground()
:myBackground("data/images/backgrounds/space.bmp", 1)
{
	for(int i = 0; i < 100; i++) {
		Star *star = new Star();
		star->x = rand()%320;
		star->y = rand()%240;
		star->speed = ((rand()%255)/255.0f);
		myStars.push_back(star);
	}
}

void SpaceBackground::onRender( BITMAP *dest )
{
	myBackground.drawFrame(dest, 0, 0, 0);
	for(unsigned int i = 0; i < myStars.size(); i++) {
		Star *star = myStars.at(i);
		putpixel(dest, star->x, star->y, makecol(star->speed * 255, star->speed * 255, star->speed * 255));
	}
}

void SpaceBackground::onLogic()
{
	for(unsigned int i = 0; i < myStars.size(); i++) {
		Star *star = myStars.at(i);
		star->x += (1 + star->speed) * 0.4;
		if(star->x > 320) {
			star->x = 0;
			star->y = rand()%240;
			star->speed = ((rand()%255)/255.0f);
		}
	}
}
