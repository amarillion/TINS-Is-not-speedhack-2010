#pragma once

class Music
{
public:
	static void init();

	static void playSong(const std::string& file);
	static void registerSong(const std::string& file, float volume);
	static void stop();
	static void onLogic();

private:
	Music();

	static std::map<std::string, float> mySongVolumes;
};