#pragma once

#include "Entity.hpp"

class Font;

class PortalEntity : public Entity {
public:
	PortalEntity();

	virtual void onLogic();

	virtual void onRender(BITMAP *dest);

	virtual float getZOrder();

	virtual bool isToBeDestroyed();

	virtual int getX();

	virtual int getY();

private:
	class Star {
	public:
		float x;
		float y;
		float speed;
	};
	bool isBetween(int start, int end);
	int myFrameCounter;
	int myFlashState;
	std::vector<Star *> myStars;
	std::vector<Star *> myLines;
};