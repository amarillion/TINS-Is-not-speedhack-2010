#include "Precompiled.hpp"
#include "MainMenu.hpp"
#include "ScreenManager.hpp"
#include "Input.hpp"
#include "Resource.hpp"
#include "Sound.hpp"
#include "Constants.hpp"
#include "Battle.hpp"
#include "SetupBattle.hpp"
#include "Music.hpp"

MainMenu::MainMenu()
:Screen(false)
,myBackground("data/images/gui_mainmenu_background.bmp", 1)
,myLogo("data/images/gui_mainmenu_logo.bmp", 1)
,myHand("data/images/gui_hand.bmp")
,mySelectedIndex(0)
{
	myFont = Resource::getFont("data/images/font.bmp");
}

void MainMenu::onLogic()
{
	if (ScreenManager::getTop() != this)
	{
		return;
	}

	Music::playSong("data/music/menu.xm");

	if (Input::isPressed(Button_Up))
	{
		mySelectedIndex--;
		if (mySelectedIndex < 0)
			mySelectedIndex = 0;
		else
			Sound::playSample("data/sound/gui_select.wav");
	}
	if (Input::isPressed(Button_Exit))
	{
		ScreenManager::exit(this);
	}
	if (Input::isPressed(Button_Down))
	{
		mySelectedIndex++;
		if (mySelectedIndex > 1)
			mySelectedIndex = 1;
		else
			Sound::playSample("data/sound/gui_select.wav");
	}
	if (Input::isPressed(Button_Action))
	{
		if (mySelectedIndex == 0)
		{
			ScreenManager::add(SetupBattle());
		}
		else if (mySelectedIndex == 1)
		{
			ScreenManager::exit(this);
		}
	}

}

void MainMenu::onDraw( BITMAP* buffer )
{
	rectfill(buffer, 0, 0, 320, 240, 0);
	myBackground.drawFrame(buffer, 0, 0, 0);
	myLogo.drawFrame(buffer, 0, 0, 30);

	myFont->draw(buffer, "Start Game", 120, 130);
	myFont->draw(buffer, "Exit", 120, 130 + myFont->getHeight());

	myHand.drawFrame(buffer, 0, 108, 130 + myFont->getHeight()*mySelectedIndex);
}


