#include "Precompiled.hpp"
#include "HackEntity.hpp"
#include "Resource.hpp"
#include "Util.hpp"

HackEntity::HackEntity(int aTimeToShow) 
: myTimeToShow(aTimeToShow)
, myFrameCounter(0)
{
	std::string fileAsString = loadFile("data/Battle.cpp");
	myLinesOfCode = tokenize(fileAsString, "\n");
	myFont = Resource::getFont("data/images/font.bmp", makecol(30, 80, 30));
}

void HackEntity::onLogic()
{
	myFrameCounter++;
	/*
	if(myFrameCounter % 10 == 0) {
		ParticleEntity* pe =new ParticleEntity("data/images/attack.bmp", 4, 1.0f, x, y, this->getZOrder() + 1);
		myBattle->addEntity(pe);
	}
	*/
}

void HackEntity::onRender( BITMAP *dest )
{
	int maxLines = myLinesOfCode.size();

	int startIndex = (myFrameCounter/3) % maxLines;
	for(int i = 0; i < 14; i++) {
		int currIndex = (startIndex + i) % maxLines;
		std::string currLine = myLinesOfCode.at(currIndex);
		myFont->draw(dest, currLine, getX(), getY() + i*myFont->getHeight(), Blending_Add );
	}
}

float HackEntity::getZOrder()
{
	return 1000;
}

bool HackEntity::isToBeDestroyed()
{
	return myFrameCounter > myTimeToShow;
}

int HackEntity::getX()
{
	return 0;
}

int HackEntity::getY()
{
	return 0;
}