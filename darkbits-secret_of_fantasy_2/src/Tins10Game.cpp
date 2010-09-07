#include "Precompiled.hpp"
#include "Tins10Game.hpp"
#include "Input.hpp"
#include "Constants.hpp"
#include "Music.hpp"
#include "ScreenManager.hpp"
#include "SplashScreen2.hpp"
#include "Battle.hpp"
#include "Sound.hpp"
#include "MainMenu.hpp"
#include "SetupBattle.hpp"
#include "ActionTriggerLibrary.hpp"

Tins10Game::Tins10Game()
:Game("db-tins10", false)
{
	Input::registerKey(Button_ToggleFullscreen, KEY_F);
	Input::registerKey(Button_ToggleSlowMotion, KEY_S);
	Input::registerKey(Button_Exit, KEY_ALT);
	Input::registerKey(Button_ToggleVSync, KEY_V);
	Input::registerKey(Button_Action, KEY_LCONTROL);
	Input::registerKey(Button_Up, KEY_UP);
	Input::registerKey(Button_Down, KEY_DOWN);
	Input::registerKey(Button_Left, KEY_LEFT);
	Input::registerKey(Button_Right, KEY_RIGHT);
	Input::registerKey(Button_ForceQuit, KEY_ESC);
	Input::registerKey(Button_All, KEY_A);

	ActionTriggerLibrary::init();

	Sound::registerSample("data/sound/gui_select.wav", 255);
	Sound::registerSample("data/sound/gui_deselect.wav", 255);
	Sound::registerSample("data/sound/attack.wav", 255);
	Sound::registerSample("data/sound/ice2.wav", 255);

	Music::registerSong("data/music/menu.xm", 0.8f);
	Music::registerSong("data/music/splash.xm", 1.0f);
	Music::registerSong("data/music/splash2.xm", 2.0f);
	Music::registerSong("data/music/per3.xm", 1.0f);
	Music::registerSong("data/music/olof6.xm", 1.0f);
	Music::registerSong("data/music/olof7.xm", 1.0f);
	Music::registerSong("data/music/gameover.xm", 1.0f);

//	Battle *battle = new Battle();

	
	ScreenManager::add(new MainMenu());
	ScreenManager::add(new SplashScreen2());

	//ScreenManager::add(SetupBattle());
	//ScreenManager::add(battle);
	
// 	battle->enqueueFight(createFight1());
// 	battle->enqueueFight(createFight2());
}

void Tins10Game::onLogic()
{
	if (Input::isPressed(Button_ToggleFullscreen))
	{
		toggleFullscreen();
	}

	if (Input::isPressed(Button_ToggleSlowMotion))
	{
		toggleSlowMotion();
	}

	if (Input::isPressed(Button_ToggleVSync))
	{
		toggleVSync();
	}

	if (Input::isPressed(Button_ForceQuit) || ScreenManager::isEmpty())
	{
		halt();
	}
}
