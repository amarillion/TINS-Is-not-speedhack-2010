#pragma once

#include "Entity.hpp"

class EffectBase: public Entity
{
public:
	EffectBase(int x, int y, float zOrder);

	float getZOrder();

	bool isToBeDestroyed();

	int getX();

	int getY();

protected:
	bool myIsDone;

private:
	int myX;
	int myY;

	float myZOrder;
};