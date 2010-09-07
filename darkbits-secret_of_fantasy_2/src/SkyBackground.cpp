#include "Precompiled.hpp"
#include "SkyBackground.hpp"
#include "Resource.hpp"

SkyBackground::SkyBackground()
{
	myBackgroundImage = Resource::getBitmap("data/images/backgrounds/grass.bmp");
	for(int i = 0; i < 4; i++) {
		myBirds.push_back(new Bird(rand()%320, rand()%30, 0.05 + ((rand()%100)/(float)100)*0.2));
	}
	
}

SkyBackground::~SkyBackground()
{

}

void SkyBackground::onLogic()
{
	for(std::vector<Bird *>::iterator it = myBirds.begin(); it != myBirds.end(); it++) {
		Bird *bird = (*it);
		bird->onLogic();
		if(bird->myX > 330) {
			bird->myRealX = -5;
			bird->myY = rand()%30;
			bird->mySpeed = 0.05 + ((rand()%100)/(float)100)*0.2;
		
		}
	}
}

void SkyBackground::onRender( BITMAP *dest )
{
	draw_sprite(dest, myBackgroundImage, 0, 0);
	for(std::vector<Bird *>::iterator it = myBirds.begin(); it != myBirds.end(); it++) {
		Bird *bird = (*it);
		bird->onRender(dest);
	}
}
