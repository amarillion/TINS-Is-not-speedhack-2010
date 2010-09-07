#include <assert.h>
#include "settings.h"
#include <allegro.h>
#include <string.h>

const char *config_keys[NUM_BUTTONS] = 
{
	"key_up", "key_down", "key_left", "key_right", "key_action"
};

Settings::Settings()
{
   // these settings are the default
	// they are overriden by the info in xelda.cfg
	// and then by command line parameters
	music = true;
	soundOn = true;
#ifdef DEBUG
	// default in debug version
	windowed = true;
	fpsOn = true;
#else
	// default in release version
	windowed = false;
	fpsOn = false;
#endif
	button[btnUp].setScancode (KEY_UP);
	button[btnDown].setScancode (KEY_DOWN);
	button[btnLeft].setScancode (KEY_LEFT);
	button[btnRight].setScancode (KEY_RIGHT);
	button[btnAction].setScancode (KEY_SPACE);
	button[btnAction].setAltcode (KEY_ENTER);	
}

void Settings::getFromConfig()
{
	music = get_config_int ("tins10", "music", music);
	soundOn = get_config_int ("tins10", "sound", soundOn);
	fpsOn = get_config_int ("tins10", "fps", fpsOn);
	windowed = get_config_int ("tins10", "windowed", windowed);
		
	for (int i = 0; i < NUM_BUTTONS; ++i)
	{
		button[i].setScancode (get_config_int ("tins10", config_keys[i],
			button[i].getScancode()));
	}
}	

void Settings::getFromArgs(int argc, const char *const *argv)
{
		// parse command line arguments
	int i;
	for (i = 1; i < argc; i++)
	{
		if (strcmp (argv[i], "-nosound") == 0)
		{
			soundOn = false;
		}
		else if (strcmp (argv[i], "-windowed") == 0)
		{
			windowed = true;
		}
		else if (strcmp (argv[i], "-fullscreen") == 0)
		{
			windowed = false;
		}
		else if (strcmp (argv[i], "-showfps") == 0)
		{
			fpsOn = true;
		}
		else
		{
			allegro_message (
				"warning: Couldn't parse a command line argument %i\n%s",		
				i, argv[i]);
		}			
	}
}

void Settings::setSoundOn(bool value)	
{
	if (value != soundOn)
	{
		soundOn = value;
	}			
}

void Settings::setMusicOn(bool value)	
{
	if (value != music)
	{
		music = value;
	}			
}
