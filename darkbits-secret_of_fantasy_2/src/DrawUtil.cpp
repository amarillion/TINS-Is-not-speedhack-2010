#include "Precompiled.hpp"
#include "DrawUtil.hpp"


bool DrawUtil::dissolveBitmap( BITMAP *dest, BITMAP *src, int x, int y, float amount)
{
	bool result = true;
	int row;
	for (row = 0; row < src->h; row++) {
		float t = amount - ((float)row / (float)src->h);
		if (t <= 0.0f) {
			break;
		}
		if ((row % 2) == 1) {
			continue;
		}
		int offset = (int)(t * t * DissolveHeight * 40);
		if (offset > DissolveHeight) {
			continue;
		}
		masked_blit(src, dest, 0, row, x, y - offset + row, src->w, 1);
		result = false;
	}

	masked_blit(src, dest, 0, row, x, y + row, src->w, src->h - row);
	return result;
}