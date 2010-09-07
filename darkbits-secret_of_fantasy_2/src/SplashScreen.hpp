#pragma once

#include "Screen.hpp"

#include <allegro.h>

class SplashScreen: public Screen
{
public:
	SplashScreen();
    void onLogic();
    void onDraw(BITMAP* aBuffer);

protected:
    BITMAP* mDarkbitsLogoBitmap;
    int mFrameCounter;
};