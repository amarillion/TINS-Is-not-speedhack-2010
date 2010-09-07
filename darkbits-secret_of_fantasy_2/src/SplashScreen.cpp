#include "Precompiled.hpp"
#include "SplashScreen.hpp"
#include "Resource.hpp"
#include "Music.hpp"

SplashScreen::SplashScreen()
    : Screen(false),
	  mFrameCounter(0)
{
    mDarkbitsLogoBitmap = Resource::getBitmap("data/images/darkbitslogo_by_haiko.bmp");
}

void SplashScreen::onLogic()
{	
	if (mFrameCounter > 300)
	{
		exit();
		return;
	}

	Music::playSong("data/music/splash.xm");
    mFrameCounter++;
}

void SplashScreen::onDraw(BITMAP* aBuffer)
{
    clear_to_color(aBuffer, makecol(255,255,255));
    
	float stretch = std::cos(mFrameCounter*0.4f) / (mFrameCounter * 0.2f);

	if (mFrameCounter > 150)
	{
		stretch = 0;
	}

	int w = mDarkbitsLogoBitmap->w * std::pow(2.0f, stretch);

	if (w <= 0)
	{
		w = 1;
	}

	int h = (mDarkbitsLogoBitmap->h * mDarkbitsLogoBitmap->w) / w;
		

    masked_stretch_blit(mDarkbitsLogoBitmap, 
                        aBuffer, 
                        0, 
                        0, 
                        mDarkbitsLogoBitmap->w,
                        mDarkbitsLogoBitmap->h,
                        160 - w / 2, 
                        120 - h / 2,
                        w,
                        h);
}
