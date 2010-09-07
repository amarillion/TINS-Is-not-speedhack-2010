#pragma once

class Sound
{
public:
	static void init();
	static void registerSample(const std::string& aFilename, int aVolume);
	static void playSample(const std::string& aFilename);
	static void onLogic();
private:
	Sound();
	class Sample
	{
	public:
		SAMPLE* mySample;
		int myVolume;
	};

	static std::set<std::string> mySamplesToPlay;
	static std::map<std::string, Sample> mySamples;
};