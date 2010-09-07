#ifndef ENGINE_H
#define ENGINE_H

#include <allegro.h>
#include "button.h"
#include "settings.h"
#include "resources.h"
#include "layer.h"
#include <list>
#include "menu.h"
#include "game.h"
#include "background.h"
#include "code.h"

using namespace std;

class Main;
class IReplay;

class Engine
{
private:
	
	Main *parent;
	
	bool debug;
	
	Settings *settings;
	Resources *resources;
	
	string gameover_message;	
	Main *getParent() { return parent; }
	
	list<Layer*> layers;	
	Menu menu;
	Background menubg;
	Game game;
	
	ViewPort view;
	Code code;

	IReplay *replay;
	bool recording;

	Button btnScreenshot;
#ifdef DEBUG
	Button btnAbort;
	Button btnDebugMode;
#endif
	int event;
public:
	enum { E_NONE, E_MAINMENU, E_FINISHED, E_STARTGAME, E_PAUSE, E_RESUME, E_QUIT, E_VIEWCODE, E_STARTREPLAY };
	Engine (Main *p);
	Game *getGame() { return &game; }
	Resources *getResources() { return resources; }
	void init(); // call once during startup
	
	// NB: you can't set a new event while the previous one wasn't processed yet
	void setEvent(int code)  { assert (event == E_NONE); event = code; clear_keybuf(); };
	void handleEvent();
		
	void draw (BITMAP *buffer);
	void update ();
	void spawn();
	
	void done(); // stop music
	bool isDebug () { return debug; }

	int getCounter ();
	int getMaxZOrder ();
		
	void playSample (SAMPLE *s);
	void playSample (const char * name);

	Button* getButton();	

	void saveReplay (int i);
};

#endif
