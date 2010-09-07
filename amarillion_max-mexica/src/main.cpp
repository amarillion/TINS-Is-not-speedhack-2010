#include <assert.h>
#include "main.h"
#include "color.h"
#include <alfont.h>

volatile int Main::counterMsec = 0;

void Main::timer_handler()
{
	Main::counterMsec += 5;
}

END_OF_FUNCTION(timer_handler);

Main::Main() : engine (this), settings()
{
	buffer = NULL;
	last_fps = 0;
	lastUpdateMsec = 0;
	frame_count = 0;
	frame_counter = 0;
	state = GAME;
}

int Main::init(int argc, const char *const *argv)
// returns 1 on success, 0 on failure
{
	LOCK_VARIABLE(Main::counterMsec);
	LOCK_FUNCTION(timer_handler);
	if (allegro_init () < 0)
	{
		allegro_message ("init failed - %s", allegro_error);
		return 0;
	}

	override_config_file ("tins10.cfg");
	settings.getFromConfig();
	settings.getFromArgs (argc, argv);

	if (install_keyboard () < 0)
	{
		allegro_message ("install keyboard failed");
		return 0;
	}
	initKeyboard(); // install custom keyboard handler

	if (install_timer() < 0)
	{
		allegro_message ("install timer failed");
		return 0;
	}
	set_volume_per_voice (1);
	if (settings.isSoundOn() && install_sound(DIGI_AUTODETECT, MIDI_NONE, NULL) < 0)
	{
		// could not get sound to work
		settings.setSoundOn(false);
		allegro_message ("Could not initialize sound. Sound is turned off.\n%s\n", allegro_error);
	}
	if (install_int (timer_handler, 5) < 0)
	{
		allegro_message ("installation of timer handler failed");
		return 0;
	}
	
	int result = alfont_init ();
	if (result != 0)
	{
		allegro_message ("Installation of alfont failed with error code %i", result);
		return 0;
	}

	const int DEFAULT_DEPTH = 16;
	int depth = DEFAULT_DEPTH;
	// in windowed mode, use the desktop depth if possible
	if (settings.windowed)
	{
		int depth = desktop_color_depth();
		if (depth == 0) depth = DEFAULT_DEPTH; // could not be determined, use default.
	}
	set_color_depth(depth);

	const int RES_NUM = 2;

	const int PREFERRED_W = 1024;
	const int PREFERRED_H = 640;

	const int RESOLUTIONS[RES_NUM][2] = {
			{PREFERRED_W, PREFERRED_H},
			{1024, 768}
	};

	bool success = false;
	int gfx_driver = settings.windowed ? GFX_AUTODETECT_WINDOWED : GFX_AUTODETECT_FULLSCREEN;
	for (int i = 0; i < RES_NUM; ++i)
	{
		if (set_gfx_mode(gfx_driver, RESOLUTIONS[i][0], RESOLUTIONS[i][1], 0, 0)==0)
		{
			success = true;
			break;
		}
	}

	if (!success)
	{
		allegro_message("Unable initialize graphics module\n%s\n", allegro_error);
		return 0;
	}

	buffer = create_bitmap(PREFERRED_W, PREFERRED_H);
	if (!buffer)
	{
		allegro_message ("Error creating background buffer");
		return 0;
	}
	if (install_mouse() == -1)
	{
		allegro_message ("could not install mouse");
		set_gfx_mode (GFX_TEXT, 0, 0, 0, 0);
		allegro_exit();
		return 0;
	}
	set_window_title ("Mexica");
	if (!resources.init())
	{
		allegro_message ("Could not load all resources!");
		return 0;
	}
	engine.init();
	return 1;
}

void Main::run()
{
	while (state != QUIT)
	{
		// wait until it's time for the next logic update
		while ((counterMsec - lastUpdateMsec) < logicIntervalMsec)
		{
			rest(1);
		}

		while ((counterMsec - lastUpdateMsec) >= logicIntervalMsec)
		{
			lastUpdateMsec += logicIntervalMsec;
			int update_start = counterMsec;

			engine.update();

			// check to see if update takes too long
			if (counterMsec - update_start > 40)
			{
				// we probably can't catch up anymore.
				// move update_counter ahead, or we have to
				// catch up even more next loop
				lastUpdateMsec = counterMsec;
				break;
			}
		}

		engine.draw(buffer);

        if (settings.fpsOn)
		{
			textprintf_ex (buffer, font, 0, 0,
				  WHITE, -1, "fps: %d", last_fps);
		}

		blit (buffer, screen, 0, 0, 0, 0, buffer->w, buffer->h);

		if ((counterMsec - frame_counter) > 1000)
		{
			last_fps = frame_count;
			frame_count = 0;
			frame_counter = counterMsec;
		}
		frame_count++;
	}
}

void Main::stop()
{
	state = QUIT;
}

Main::~Main()
{
	if (buffer) destroy_bitmap (buffer);
	engine.done();
	alfont_exit();
}

int main(int argc, const char *const *argv)
{
	Main m;
	if (m.init(argc, argv))
	{
		m.run();
	}
	return 0;
} END_OF_MAIN();
