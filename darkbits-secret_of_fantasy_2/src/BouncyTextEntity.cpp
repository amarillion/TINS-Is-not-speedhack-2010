#include "Precompiled.hpp"
#include "BouncyTextEntity.hpp"
#include "Resource.hpp"
#include "Util.hpp"

BouncyTextEntity::BouncyTextEntity(const std::string &text, int x, int y, unsigned int color, int zOffset)
{
	myFont = Resource::getFont("data/images/font.bmp", color);
	myText = text;
	myX = x;
	myY = y;
	myFrameCounter = 0;
	myZOffset = zOffset;
}

void BouncyTextEntity::onLogic()
{
	++myFrameCounter;
}

double sqr(double a) {
	return a*a;
}

void BouncyTextEntity::onRender( BITMAP *dest )
{
	int offset = myFont->getWidth(myText) / 2;
	for(int i = 0; i <  (int)myText.size(); ++i) {
		
		char currChar = myText[i];
		int x = myFont->getWidth(myText.substr(0, i)) + getX() - offset;
		double t = (myFrameCounter - i*2)*0.15;

		if (t < 0.0) continue;

		double f1 = 1 - sqr(t - 1);
		double f2 = 0.5 - sqr(t - 2 - sqrt(0.5));
		double h = std::max(0.0, std::max(f1, f2))*12;

		double y = getY() - h;
		
		myFont->drawGlyph(dest, currChar, x, y);
	}
	
}

float BouncyTextEntity::getZOrder()
{
	return 10000 + myZOffset;
}

bool BouncyTextEntity::isToBeDestroyed()
{
	return myFrameCounter > 75;
}

int BouncyTextEntity::getX()
{
	return myX;
}

int BouncyTextEntity::getY()
{
	return myY;
}