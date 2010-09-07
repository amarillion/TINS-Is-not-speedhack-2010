#include "Precompiled.hpp"
#include "Sound.hpp"
#include "Resource.hpp"

std::set<std::string> Sound::mySamplesToPlay;
std::map<std::string, Sound::Sample> Sound::mySamples;


Sound::Sound()
{

}

void Sound::init()
{
	install_sound(DIGI_AUTODETECT, MIDI_NONE, NULL);
}

void Sound::registerSample( const std::string& aFilename, int aVolume )
{
	Sample s;
	s.mySample = Resource::getSample(aFilename);
	s.myVolume = aVolume;
	mySamples[aFilename] = s;
}

void Sound::playSample( const std::string& aFilename )
{
	mySamplesToPlay.insert(aFilename);
}

void Sound::onLogic()
{
	for (std::set<std::string>::iterator it = mySamplesToPlay.begin(); it != mySamplesToPlay.end(); it++)
	{
		Sample s = mySamples[(*it)];
		play_sample(s.mySample, s.myVolume, 128, 1000, 0);
	}

	mySamplesToPlay.clear();
}