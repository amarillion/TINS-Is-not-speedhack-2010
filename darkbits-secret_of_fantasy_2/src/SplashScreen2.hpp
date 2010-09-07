#pragma once

#include "Screen.hpp"

class SplashScreen2: public Screen
{
public:
	SplashScreen2();
    void onLogic();
    void onDraw(BITMAP* aBuffer);

protected:
	int myFrameCounter;
    BITMAP* myDarkbitsLogo;
	BITMAP* myDarkbitsLogoBlackAndWhite;
	BITMAP* myDarkbitsLogoGlow;
	BITMAP* mySoftware2010;
	BITMAP* myDarkbitsLogoBlink;
};