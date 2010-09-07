#include <assert.h>
#include "engine.h"
#include "main.h"
#include "color.h"
#include "screenshot.h"

int Engine::getCounter ()
{
	return parent->getCounter();
}

Engine::Engine (Main *p) : layers (), menu (*this), menubg (*this), game(*this),
		view(), code (*this), event (E_NONE)
{
	parent = p;
	debug = false;
	resources = p->getResources();
	settings = p->getSettings();
	view.setOfst (0, 0);

	game.setViewPort(&view);
	
	layers.push_back (&game);
	layers.push_back (&menubg);
	layers.push_back (&menu);
	
	layers.push_back (&code);

	btnScreenshot.setScancode (KEY_F12);

#ifdef DEBUG
	btnAbort.setScancode (KEY_F10);
	btnDebugMode.setScancode (KEY_F11);
#endif

	setEvent(E_MAINMENU);
}

Button* Engine::getButton()
{
	return settings->getButton();
}

void Engine::init()
{
 	srand(time(0));
 	menu.init();
 	game.init();
 	code.init();
 	menubg.init(); // must be inited after game!
}

void Engine::handleEvent()
{
	switch (event)
	{
		case E_NONE:
			// do nothing;
			break;
		case E_MAINMENU:
			menu.prepare(0);
			menu.foreground();
			menubg.background();
			game.stop();
			code.stop();
			clear_keybuf();
			break;
		case E_FINISHED:
			menu.prepare(recording ? 2 : 0);
			menu.foreground();
			menubg.background();
			game.stop();
			code.stop();
			clear_keybuf();
			break;
		case E_STARTGAME:
			menu.stop();
			menubg.stop();
			recording = true;
			replay = new ReplayRecorder(getButton());
			game.initGame(replay);
			game.foreground();
			clear_keybuf();
			break;
		case E_STARTREPLAY:
			menu.stop();
			menubg.stop();
			recording = false;
			replay = new ReplayPlayer(getButton(), menu.getSelectedReplay());
			game.initGame(replay);
			game.foreground();
			clear_keybuf();
			break;
		case E_QUIT:
			parent->stop();
			clear_keybuf();
			break;
		case E_VIEWCODE:
			menu.stop();
			menubg.stop();
			code.foreground();
			clear_keybuf();
			break;
	}
	event = E_NONE;
}

void Engine::draw (BITMAP *buffer)
{
	for (list<Layer*>::iterator i = layers.begin(); i != layers.end(); ++i)
	{
		if ((*i)->getState() != Layer::LAYER_SLEEP)
			(*i)->draw(buffer);
	}
}

void Engine::update ()
{
	if (btnScreenshot.justPressed())
	{
		screenshot();
	}

#ifdef DEBUG
	if (btnDebugMode.justPressed())
	{
		debug = !debug;
	}
	if (btnAbort.justPressed())
	{
		setEvent(E_QUIT);
	}
#endif

	handleEvent();
	
	for (list<Layer*>::iterator i = layers.begin(); i != layers.end(); ++i)
	{
		Layer::LayerState state = (*i)->getState();

		if (state == Layer::LAYER_FOREGROUND)
			(*i)->handleInput();

		if (state != Layer::LAYER_SLEEP)
			(*i)->update();
	}
}

void Engine::playSample (SAMPLE *s)
{
	if (!settings->isSoundOn()) return;
	play_sample (s, 127, 127, 1000, 0);
}

void Engine::playSample (const char *name)
{
	if (!settings->isSoundOn()) return;
	play_sample (resources->getSample(string(name)), 127, 127, 1000, 0);
}

void Engine::done()
{
}

int Engine::getMaxZOrder()
{
	int max = 0;
	for (list<Layer*>::iterator i = layers.begin(); i != layers.end(); ++i)
	{
		if ((*i)->getZOrder() > max)
		{
			max = (*i)->getZOrder();
		}
	}
	return max;
}

void Engine::saveReplay(int i)
{
	ReplayRecorder *r = dynamic_cast<ReplayRecorder*>(replay);
	assert (r);
	r->writeToFile(i);
}
