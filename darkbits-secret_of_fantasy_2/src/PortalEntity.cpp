#include "Precompiled.hpp"
#include "PortalEntity.hpp"
#include "Util.hpp"

PortalEntity::PortalEntity()
: myFrameCounter(0)
, myFlashState(0)
{
	for(int i = 0; i < 100; i++) {
		Star *star = new Star();
		star->x = rand()%320;
		star->y = rand()%240;
		star->speed = ((rand()%255)/255.0f);
		myStars.push_back(star);
	}
	for(int i = 0; i < 50; i++) {
		Star *star = new Star();
		star->x = rand()%320;
		star->y = rand()%240;
		star->speed = ((rand()%255)/255.0f);
		myLines.push_back(star);
	}
}

void PortalEntity::onLogic()
{
	myFrameCounter++;

	
	for(unsigned int i = 0; i < myStars.size(); i++) {
		Star *star = myStars.at(i);
		star->x += (1 + star->speed) * 2;
		if(star->x > 320) {
			star->x = 0;
			star->y = rand()%240;
			star->speed = ((rand()%255)/255.0f);
		}
	}

	for(unsigned int i = 0; i < myLines.size(); i++) {
		Star *star = myLines.at(i);
		star->x += (1 + star->speed) * 3.4;
		if(star->x > 320) {
			star->x = 0;
			star->y = rand()%240;
			star->speed = ((rand()%255)/255.0f);
		}
	}
	if(myFlashState == 0) {
		float probability = 0.1; //clamp(myFrameCounter/400.0f, 0.0f, 1.0f);

		if(rand()%(int)(1 + ((1-probability)*100)) == 0) {
			myFlashState = 1;
		}
	} else {
		myFlashState++;
		if(myFlashState > 7) myFlashState = 0;
	}
}
bool PortalEntity::isBetween(int start, int end) {
	return myFrameCounter > start && myFrameCounter < end;
}

void PortalEntity::onRender( BITMAP *dest )
{

	rectfill(dest, 0, 0, 320, 240, makecol(50, 50, 50));

	if(myFlashState > 0) {
		if(myFlashState < 5) {
			rectfill(dest, 0, 0, 320, 240, makecol(0, 0, 0));
		} else if (myFlashState > 5 && myFlashState < 7) {
			rectfill(dest, 0, 0, 320, 240, makecol(255, 255, 255));
		} 
	}

	for(unsigned int i = 0; i < myStars.size(); i++) {
		Star *star = myStars.at(i);
		putpixel(dest, star->x, star->y, makecol(star->speed * 255, star->speed * 255, star->speed * 255));
	}

	for(unsigned int i = 0; i < myLines.size(); i++) {
		Star *star = myLines.at(i);
		line(dest, star->x, star->y, star->x + 38*star->speed, star->y, makecol(255 * star->speed, 255 * star->speed, 255 * star->speed));
		//putpixel(dest, star->x, star->y, makecol(star->speed * 255, star->speed * 255, star->speed * 255));
	}
}

float PortalEntity::getZOrder()
{
	return 3;
}

bool PortalEntity::isToBeDestroyed()
{
	return false;
}

int PortalEntity::getX()
{
	return 0;
}

int PortalEntity::getY()
{
	return 0;
}