#include "Precompiled.hpp"
#include "Animation.hpp"

#include "Resource.hpp"
#include "Util.hpp"

Animation::Animation(const std::string& filename)
{
	BITMAP *allFrames = Resource::getBitmap(filename);
	mFrameWidth = mFrameHeight = allFrames->h;
	int count = allFrames->w / mFrameWidth;
	
	fillFramesList(allFrames, count);
}

Animation::Animation(const std::string& filename, int frames)
{
	BITMAP *allFrames = Resource::getBitmap(filename);
	mFrameWidth = allFrames->w / frames;
	mFrameHeight = allFrames->h;
	int count = frames;

	fillFramesList(allFrames, count);
}

Animation::~Animation() 
{
	unsigned int i;
	for (i = 0; i < mFrames.size(); i++) {
		destroy_bitmap(mFrames[i]);
	}
}

BITMAP *Animation::getFrame(int frame) const {
	return mFrames[frame % mFrames.size()];
}

int Animation::getFrameCount() const {
	return mFrames.size();
}

int Animation::getFrameWidth() const {
	return mFrameWidth;
}

int Animation::getFrameHeight() const {
	return mFrameHeight;
}

void Animation::drawFrame(BITMAP *dest, int frame, int x, int y, bool hflip, bool vflip, Blending blending) const 
{
	BITMAP *src = getFrame(frame);

	if (blending == Blending_None)
	{
		if (hflip && vflip) {
			draw_sprite_vh_flip(dest, src, x, y);
		} else if (hflip && !vflip) {
			draw_sprite_h_flip(dest, src, x, y);
		} else if (!hflip && vflip) {
			draw_sprite_v_flip(dest, src, x, y);
		} else {
			draw_sprite(dest, src, x, y);
		}
		return;
	}

	// Blended drawing follows
	bool addBlend = blending == Blending_Add;
	setBlending(blending);

	BITMAP *temp;

	if (hflip || vflip) {
		temp = create_bitmap(mFrameWidth, mFrameHeight);
		clear_to_color(temp, makecol(255, 0, 255));

		if (hflip && vflip) {
			draw_sprite_vh_flip(temp, src, 0, 0);
		} else if (hflip && !vflip) {
			draw_sprite_h_flip(temp, src, 0, 0);
		} else {
			draw_sprite_v_flip(temp, src, 0, 0);
		} 
	} else {
		temp = src;
	}

	if (addBlend) {
		draw_trans_sprite(dest, temp, x, y);
	} else {
		draw_lit_sprite(dest, temp, x, y, 0);
	}

	if (hflip || vflip) {
		destroy_bitmap(temp);
	}
}

void Animation::drawRotatedFrame(BITMAP *dest, int frame, int x, int y, int angle, bool vflip) const 
{
	BITMAP *src = getFrame(frame);

	if (vflip) {
		rotate_sprite_v_flip(dest, src, x, y, itofix(angle));
	} else {
		rotate_sprite(dest, src, x, y, itofix(angle));
	}
}

void Animation::fillFramesList(BITMAP *allFrames, int count)
{
	mFrames.reserve(count);
	
	int i;
	for (i = 0; i < count; i++) {
		BITMAP *frame = create_sub_bitmap(allFrames, i * mFrameWidth, 0, mFrameWidth, mFrameHeight);
		mFrames.push_back(frame);
	}
}