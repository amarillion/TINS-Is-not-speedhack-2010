#include "Precompiled.hpp"
#include "Blending.hpp"
#include "Util.hpp"

void setBlending(Blending aBlending)
{
	switch (aBlending) {
	case Blending_Frozen:
		set_blender_mode(ice15, ice16, ice24, 0, 0, 0, 0);
		break;
	case Blending_Add:
		set_blender_mode(add15, add16, add24, 0, 0, 0, 0);
		break;
	case Blending_Lighten:
		set_blender_mode(lighten15, lighten16, lighten24, 0, 0, 0, 0);
		break;
	}
}

 unsigned long add24(unsigned long x, unsigned long y, unsigned long n)
{
	unsigned long rb = (x & 0xff00ff) + (y & 0xff00ff);
	unsigned long f = rb & 0x1000100;
	unsigned long g = (x & 0xff00) + (y & 0xff00);
	f |= g & 0x10000;
	return ((rb | g) - f) | (f - (f >> 8));
}

 unsigned long add16(unsigned long x, unsigned long y, unsigned long n)
{
	const unsigned long rmask = 0x001F;
	const unsigned long gmask = 0x07E0;
	const unsigned long bmask = 0xF800;
	unsigned long r = min((x & rmask) + (y & rmask), rmask);
	unsigned long g = min((x & gmask) + (y & gmask), gmask);
	unsigned long b = min((x & bmask) + (y & bmask), bmask);
	return r | g | b;
}

 unsigned long add15(unsigned long x, unsigned long y, unsigned long n)
{
	const unsigned long rmask = 0x001F;
	const unsigned long gmask = 0x03E0;
	const unsigned long bmask = 0x7C00;
	unsigned long r = min((x & rmask) + (y & rmask), rmask);
	unsigned long g = min((x & gmask) + (y & gmask), gmask);
	unsigned long b = min((x & bmask) + (y & bmask), bmask);
	return r | g | b;
}

 unsigned long ice24(unsigned long x, unsigned long y, unsigned long n)
{
	unsigned long v = (y & 0xff) + ((y >> 8) & 0xff) + ((y >> 16) & 0xff);
	unsigned long r = (v / 3) << 16;
	unsigned long g = min(v / 2, 0xff) << 8;
	unsigned long b = min(v + 64, 0xff);
	return r | g | b;
}

 unsigned long ice16(unsigned long x, unsigned long y, unsigned long n)
{
	unsigned long v = (y & 0x1f) + ((y >> 6) & 0x1f) + ((y >> 11) & 0x1f);
	unsigned long r = (v / 3) << 11;
	unsigned long g = min(v / 2, 0x1f) << 6;
	unsigned long b = min(v + 8, 0x1f);
	return r | g | b;
}

 unsigned long ice15(unsigned long x, unsigned long y, unsigned long n)
{
	unsigned long v = (y & 0x1f) + ((y >> 5) & 0x1f) + ((y >> 10) & 0x1f);
	unsigned long r = (v / 3) << 10;
	unsigned long g = min(v / 2, 0x1f) << 5;
	unsigned long b = min(v + 8, 0x1f);
	return r | g | b;
}

 unsigned long lighten24(unsigned long x, unsigned long y, unsigned long n)
{
	return ((y & 0xffffff) == 0xffffff) ? 0 : (y ? 0xffffff : 0);
}

 unsigned long lighten16(unsigned long x, unsigned long y, unsigned long n)
{
	return y ? 0xffff : 0;
}

 unsigned long lighten15(unsigned long x, unsigned long y, unsigned long n)
{
	return y ? 0xffff : 0;
}