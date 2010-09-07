#pragma once

#include "BackgroundEntity.hpp"
#include "Animation.hpp"

class SpaceBackground: public BackgroundEntity
{
public:
	SpaceBackground();
	void onRender(BITMAP *dest);
	void onLogic();
private:
	class Star {
	public:
		float x;
		float y;
		float speed;
	};
	Animation myBackground;
	std::vector<Star *> myStars;
};