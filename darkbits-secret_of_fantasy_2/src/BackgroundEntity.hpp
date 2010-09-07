#pragma once

#include "Entity.hpp"

class BackgroundEntity : public Entity {
	virtual float getZOrder();

	virtual int getX();

	virtual int getY();

	virtual bool isToBeDestroyed();

};