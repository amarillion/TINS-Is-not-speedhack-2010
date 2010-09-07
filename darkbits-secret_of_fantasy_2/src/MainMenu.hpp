#pragma once

#include "Screen.hpp"
#include "Font.hpp"
#include "Animation.hpp"

class Battle;

class MainMenu: public Screen
{
public:
	MainMenu();
	void onLogic();
	void onDraw(BITMAP* buffer);
private:

	Animation myHand;
	Animation myBackground;
	Animation myLogo;
	Font* myFont;
	int mySelectedIndex;
};