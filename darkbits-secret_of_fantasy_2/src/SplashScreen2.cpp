#include "Precompiled.hpp"
#include "SplashScreen2.hpp"
#include "Music.hpp"
#include "Resource.hpp"

SplashScreen2::SplashScreen2()
    : Screen(false),
	myFrameCounter(0)
{
    myDarkbitsLogo = Resource::getBitmap("data/images/darkbitslogo.bmp");
	myDarkbitsLogoBlackAndWhite = Resource::getBitmap("data/images/darkbitslogo_black_and_white.bmp");
	myDarkbitsLogoGlow = Resource::getBitmap("data/images/darkbitslogo_glow.bmp");
	mySoftware2010 = Resource::getBitmap("data/images/software_2010.bmp");
	myDarkbitsLogoBlink = Resource::getBitmap("data/images/darkbitslogo_blink.bmp");
}

void SplashScreen2::onLogic()
{	
	 myFrameCounter++;

	if (myFrameCounter < 50)
		return;

	if (myFrameCounter == 51)
	{
		 Music::playSong("data/music/splash2.xm");
	}

	if (myFrameCounter == 650)
	{
		Music::stop();
	}

	if (myFrameCounter > 700)
	{
		exit();
		return;
	}
}

void SplashScreen2::onDraw(BITMAP* aBuffer)
{    
	int flashStart = 100;
	int flashEnd = 120;

	if (myFrameCounter < flashStart 
		|| myFrameCounter > 650)
	{
		clear_to_color(aBuffer, 0);
		return;
	}
	if ((myFrameCounter < flashEnd && myFrameCounter % 5 < 2))
	{
		masked_blit(myDarkbitsLogoBlackAndWhite, aBuffer, 
					0, 
					0, 
					0, 
					0, 
					myDarkbitsLogo->w, 
					myDarkbitsLogo->h);
	}
	else if (myFrameCounter < flashEnd && myFrameCounter % 5 >= 2
			|| myFrameCounter >= flashEnd)
	{
		if (myFrameCounter > 140 && myFrameCounter < 200)
		{
			static int x = 0;
			masked_blit(myDarkbitsLogoGlow, 
						aBuffer, 
						0, 
						0, 
						(x * 6) - 120, 
						0, 
						myDarkbitsLogo->w, 
						myDarkbitsLogo->h);
			x++;
		}
		else
		{
			clear_to_color(aBuffer, makecol(166, 166, 166));
		}

		masked_blit(myDarkbitsLogo, 
					aBuffer, 
					0, 
					0, 
					0, 
					0, 
					myDarkbitsLogo->w, 
					myDarkbitsLogo->h);
	}
	else
	{
		clear_to_color(aBuffer, 0);
	}

	if ((myFrameCounter > 290 && myFrameCounter < 330 && rand() % 3 == 0)
			|| myFrameCounter >= 330)
		masked_blit(mySoftware2010,
					aBuffer, 
					0, 
					0, 
					320 / 2 - mySoftware2010->w / 2, 
					180, 
					mySoftware2010->w, 
					mySoftware2010->h);

	if (myFrameCounter > 190 && myFrameCounter < 200)
		masked_blit(myDarkbitsLogoBlink, aBuffer, 0, 0, 70, 67, myDarkbitsLogoBlink->w, myDarkbitsLogoBlink->h);
	if (myFrameCounter > 210 && myFrameCounter < 220)
		masked_blit(myDarkbitsLogoBlink, aBuffer, 0, 0, 263, 114, myDarkbitsLogoBlink->w, myDarkbitsLogoBlink->h);
}