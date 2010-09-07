#ifndef MAIN_H
#define MAIN_H

#include <allegro.h>
#include "resources.h"
#include "engine.h"
#include "settings.h"

class Main
{
private:
	BITMAP *buffer;
	Resources resources;
	Engine engine;
	Settings settings;
	
	int frame_count;
	int frame_counter;
	int last_fps;
	int lastUpdateMsec;
	
	enum MainState {GAME = 1, QUIT = 2 };
	
	//state machine
	MainState state; 
	// start menu, resume menu, playing, quitting
	
	static void timer_handler();
	static volatile int counterMsec;
public:

	static const int logicIntervalMsec = 20;

	int getCounter () { return counterMsec; }
	Settings *getSettings() { return &settings; }
	Resources* getResources () { return &resources; }
	
	Main ();
	int init(int argc, const char *const *argv);
	void run();
	void done();
	void stop();
	~Main();		
};

#endif
