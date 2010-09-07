#include "Precompiled.hpp"
#include "GameOverScreen.hpp"
#include "Resource.hpp"
#include "Music.hpp"
#include "Input.hpp"
#include "Constants.hpp"
#include "ScreenManager.hpp"
#include "Battle.hpp"

GameOverScreen::GameOverScreen()
: Screen(true)
{
	myFont = Resource::getFont("data/images/font.bmp");
	
}

void GameOverScreen::onLogic()
{
	Music::playSong("data/music/gameover.xm");
	if (Input::isPressed(Button_Exit) || Input::isPressed(Button_Action))
	{
		ScreenManager::exit(this);
	}
}

void GameOverScreen::onDraw( BITMAP* buffer )
{
	for(int y = 0; y < 240; y+=1) {
		int xStart = (y % 2) == 0 ? 0 : 1;
		for(int x = xStart; x < 320; x+=2) {
			putpixel(buffer, x, y, makecol(0, 0, 0));
		}
	}
	myFont->drawCenter(buffer, "GAME OVER", 0, 0, 320, 200);
}