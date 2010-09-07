#pragma once

#include "Font.hpp"

class Resource
{
public:
    static void init();
    static void destroy();
    static BITMAP* getBitmap(const std::string& filename, unsigned int color);
	static BITMAP* getBitmap(const std::string& filename);
	static SAMPLE* getSample(const std::string& filename);
	static Font* getFont(const std::string& filename, unsigned int textColor);
	static Font* getFont(const std::string& filename);
	static std::string getRealFilename(const std::string& filename);
    
private:
    Resource();
    static std::map<std::string, BITMAP*> mBitmaps;
	static std::map<std::string, Font*> mFonts;
	static std::map<std::string, SAMPLE*> mSamples;
};