#pragma once

class DrawUtil {
public:
	static bool dissolveBitmap(BITMAP *dest, BITMAP *src, int x, int y, float amount);

private:
	static const int DissolveHeight = 10;

	DrawUtil() {};
};