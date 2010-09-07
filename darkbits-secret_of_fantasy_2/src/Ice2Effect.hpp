#pragma once

#include "EffectBase.hpp"
#include "Animation.hpp"

class Ice2Effect: public EffectBase
{
public:
	Ice2Effect(int x, int y, float zOrder);
	 void onLogic();
	 void onRender(BITMAP *dest);
private:
	int myTime;
	Animation myAnimation;
};