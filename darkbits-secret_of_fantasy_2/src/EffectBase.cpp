#include "Precompiled.hpp"
#include "EffectBase.hpp"

EffectBase::EffectBase( int x, int y , float zOrder)
:myX(x), myY(y), myZOrder(zOrder), myIsDone(false)
{

}

float EffectBase::getZOrder()
{
	return myZOrder;
}

bool EffectBase::isToBeDestroyed()
{
	return myIsDone;
}

int EffectBase::getX()
{
	return myX;
}

int EffectBase::getY()
{
	return myY;
}
