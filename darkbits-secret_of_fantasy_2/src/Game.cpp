#include "Precompiled.hpp"
#include "Game.hpp"

#include "Input.hpp"
#include "Music.hpp"
#include "Resource.hpp"
#include "ScreenManager.hpp"
#include "Time.hpp"
#include "Sound.hpp"

Game::Game(const std::string& aTitle, bool windowed)
	: myRunning(true)
	, mySlowMotion(false)
	, myVSync(false)
    , myWindowed(windowed)
	, myTitle(aTitle)
{
    allegro_init();
	
	Sound::init();
	Input::init();
	Resource::init();
	Music::init();
	Time::init();
	ScreenManager::init();
	
	initGraphics(windowed);
}

Game::~Game()
{
	ScreenManager::destroy();
	Resource::destroy();
	destroy_bitmap(myScreenBuffer);
}

void Game::initGraphics(bool windowed)
{
	int driver;
	if (myWindowed)
	{
		driver = GFX_AUTODETECT_WINDOWED;
	}
	else
	{
		driver = GFX_AUTODETECT_FULLSCREEN;
	}

    set_color_depth(32);
    if (set_gfx_mode(driver, 640, 480, 0, 0) < 0) {
	    set_color_depth(24);
	    if (set_gfx_mode(driver, 640, 480, 0, 0) < 0) {
		    set_color_depth(16);
		    if (set_gfx_mode(driver, 640, 480, 0, 0) < 0) {
			    set_color_depth(15);
			    if (set_gfx_mode(driver, 640, 480, 0, 0) < 0) 
                {
					throw std::string("Unable to set graphics mode.");
			    }
		    }
	    }
    }

	myScreenBuffer = create_bitmap(320, 240);
	setTitle(myTitle.c_str());
}

void Game::run()
{
#ifndef _DEBUG
 	try
 	{
#endif
		long frame = Time::getTick();
		int second = Time::getTick() / Time::TicksPerSecond;
		int framesPerSecond = 0;

		while (myRunning != false) 
		{
			static int tmp = 0;
			tmp++;
			if (tmp % 5 == 0 || !mySlowMotion)
			{
				Input::onLogic();
				Music::onLogic();
				ScreenManager::onLogic();
				Sound::onLogic();
				onLogic();
			}

			if (Time::getTick() - frame > 20)
			{
				frame = Time::getTick();
				std::cout << "Logic frame drop! =(" << std::endl;
			}

			frame++;

			if (frame > Time::getTick())
			{
				ScreenManager::draw(myScreenBuffer);
				if (myVSync)
				{
					vsync();
				}
				stretch_blit(myScreenBuffer, screen, 0, 0, 320, 240, 0, 0, 640, 480);

				framesPerSecond++;
			}

			while (frame > Time::getTick()) 
			{
				rest(frame - Time::getTick());
			}

			if (second != Time::getTick() / Time::TicksPerSecond) 
			{	
				second = Time::getTick() / Time::TicksPerSecond;
				framesPerSecond = 0;
			}
		}
#ifndef _DEBUG
	}
	catch (std::string e)
	{
		std::string str;
        std::ostringstream os(str);
		os << "An exception occured: \"" << e << "\".";
        allegro_message(e.c_str());
	}
	catch (std::exception e)
	{
        std::string str;
        std::ostringstream os(str);
		os << "An STD exception occured: \"" << e.what() << "\".";
        allegro_message(os.str().c_str());
	}
    catch (...)
	{
        std::string error = "An unknown exception occured (this is generally very bad).";
        allegro_message(error.c_str());
	}
#endif
}

void Game::halt()
{
	myRunning = false;
}

void Game::setTitle(const char*	format, ...)
{
	char buffer[4096];

	va_list lst;
	va_start(lst, format);
	vsprintf_s(buffer, 4096, format, lst);
	va_end(lst);

	set_window_title(buffer);
}

void Game::toggleSlowMotion()
{
	mySlowMotion = !mySlowMotion;
}

void Game::toggleVSync()
{
	myVSync = !myVSync;

	if (myVSync)
	{
		setTitle("%s | vsync", myTitle.c_str());
	}
	else
	{
		setTitle("%s", myTitle.c_str());
	}
}

void Game::toggleFullscreen()
{
	myWindowed = !myWindowed;

	int driver;
	if (myWindowed)
	{
		driver = GFX_AUTODETECT_WINDOWED;
	}
	else
	{
		driver = GFX_AUTODETECT_FULLSCREEN;
	}

    set_color_depth(32);
    if (set_gfx_mode(driver, 640, 480, 0, 0) < 0) {
	    set_color_depth(24);
	    if (set_gfx_mode(driver, 640, 480, 0, 0) < 0) {
		    set_color_depth(16);
		    if (set_gfx_mode(driver, 640, 480, 0, 0) < 0) {
			    set_color_depth(15);
			    if (set_gfx_mode(driver, 640, 480, 0, 0) < 0) 
                {
					throw std::string("Unable to set graphics mode.");
			    }
		    }
	    }
    }

	myScreenBuffer = create_bitmap(320, 240);
}