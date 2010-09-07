#pragma once

#include "Blending.hpp"

class Animation
{
public:
	Animation(const std::string& filename);
	Animation(const std::string& filename, int frames);
	~Animation();

	BITMAP *getFrame(int frame) const;
	int getFrameCount() const;
	int getFrameWidth() const;
	int getFrameHeight() const;
	void drawFrame(BITMAP *dest, int frame, int x, int y, bool hflip = false, bool vflip = false, Blending blending = Blending_None) const;
	//void drawFrame(BITMAP *dest, int frame, int x, int y, bool aHFlip, int aFillColor) const;
	void drawRotatedFrame(BITMAP *dest, int frame, int x, int y, int angle, bool vflip = false) const;

protected:
	void fillFramesList(BITMAP *allFrames, int count);

	int mFrameWidth, mFrameHeight;
	std::vector<BITMAP *> mFrames;
};