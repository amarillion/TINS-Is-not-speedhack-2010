#include "Precompiled.hpp"
#include "Music.hpp"
#include "Resource.hpp"

#include "aldumb.h"
#include "dumb.h"

static DUH *playing = NULL;
static AL_DUH_PLAYER *player;
static std::string playingFile;


std::map<std::string, float> Music::mySongVolumes;

Music::Music()
{

}

void Music::init()
{
	atexit(&dumb_exit);
	dumb_register_stdfiles();
}

void Music::playSong(const std::string& file)
{
	if (playingFile == file) 
	{
		return;
	}

	if (playing != NULL) 
	{
		al_stop_duh(player);
		unload_duh(playing);
	}

	std::string filename = Resource::getRealFilename(file);

	if (!exists(filename.c_str())) 
	{
		throw std::string("Unable to load ") + filename;
	}

	float volume = mySongVolumes[file];

	playing = dumb_load_xm_quick(filename.c_str());
	player = al_start_duh(playing, 2, 0, volume, 4096, 44100);
	playingFile = file;

	onLogic();
}

void Music::stop() 
{
	if (playing != NULL) 
	{
		al_stop_duh(player);
		unload_duh(playing);
	}
	
	playing = NULL;
	playingFile = "";
}

void Music::onLogic() 
{
	al_poll_duh(player);
}

void Music::registerSong( const std::string& file , float volume)
{
	mySongVolumes[file] = volume;
}
