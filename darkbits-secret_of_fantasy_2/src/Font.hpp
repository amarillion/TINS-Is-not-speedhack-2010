#pragma once

#include "Blending.hpp"

class Font 
{
public:
	Font(BITMAP *aGlyphImage, const std::string& aGlyphs);
	Font(BITMAP *aGlyphImage, char aStartChar, char aEndChar);

	void draw( BITMAP* aBuffer, const std::string& aText, int aX, int aY, Blending aBlending = Blending_None);
	void drawWrap(BITMAP* aBuffer, const std::string& aText, int aX, int aY, int aMaxWidth = -1, int aNumberOfCharacters = -1);
	void drawCenter(BITMAP* aBuffer, const std::string& aText, int aX, int aY, int aWidth, int aHeight);
	void drawCenterY(BITMAP* aBuffer, const std::string& aText, int aX, int aY, int aHeight);
	void drawCenterYAlignRight(BITMAP* aBuffer, const std::string& aText, int aX, int aY, int aHeight);
	void drawGlyph(BITMAP* aBuffer, char aChar, int aX, int aY);

	unsigned int getWidth(const std::string& aText);
	unsigned int getHeight() const;
private:
	void privInit(BITMAP *aGlyphImage, const std::string& aGlyphs);
	BITMAP* getBitmapForGlyph(const char glyph);
	unsigned int myHeight;
	std::map<char, BITMAP*> myGlyphToBitmap;
	
};