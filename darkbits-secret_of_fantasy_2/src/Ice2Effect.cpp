#include "Precompiled.hpp"
#include "Ice2Effect.hpp"
#include "Sound.hpp"

Ice2Effect::Ice2Effect( int x, int y, float zOrder )
:EffectBase(x, y, zOrder),
myTime(0),
myAnimation("data/images/ice2.bmp", 1)
{

}

void Ice2Effect::onLogic()
{
	myTime++;

	if (myTime == 2)
	{
		Sound::playSample("data/sound/ice2.wav");
	}
	if (myTime > 90)
	{
		myIsDone = true;
	}
}

void Ice2Effect::onRender( BITMAP *dest )
{
	int x = getX() - myAnimation.getFrameWidth() / 2;
	int y = getY() - myAnimation.getFrameHeight() / 2;

	myAnimation.drawFrame(dest, 0, x, y, false, false, Blending_Add);
	//myAnimation.drawFrame(dest, 0, x, y, false, false, Blending_Add);

	if (myTime >= 1 && myTime <= 3)
		rectfill(dest, 0, 0, 320, 240, makecol(255, 255, 255));
}
