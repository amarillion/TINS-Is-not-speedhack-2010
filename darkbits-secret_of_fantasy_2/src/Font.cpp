#include "Precompiled.hpp"
#include "Font.hpp"
#include "Resource.hpp"
#include "Util.hpp"

Font::Font(BITMAP *aGlyphImage, const std::string& aGlyphs)
{
	privInit(aGlyphImage, aGlyphs);
}

Font::Font(BITMAP *aGlyphImage, char aStartChar, char aEndChar)
{
	std::string glyphs;
	for(char theChar = aStartChar; theChar < aEndChar; theChar++)
	{
		glyphs += theChar;
	}

	privInit(aGlyphImage, glyphs);
}

void Font::privInit(BITMAP *aGlyphImage, const std::string& aGlyphs)
{
	BITMAP *glyphImage = aGlyphImage;
	int separatingColor = getpixel(glyphImage, 0, 0);
	
	int x = 0;
	int scanLine = 0;
	unsigned int currGlyphIndex = 0;
	int lastRowHeight = 0;
	char specialGlyph = (char)200;

	while(scanLine < glyphImage->h)
	{
		x = 0;
		while(x < glyphImage->w)
		{
			int color = getpixel(glyphImage, x, scanLine);
			
			if(color != separatingColor) //glyph found!
			{
				int y1 = scanLine;
				while((y1 < glyphImage->h) && getpixel(glyphImage, x, y1) != separatingColor) //find bottom of glyph
				{
					y1++;
				}
				int x1 = x;
				while((x1 < glyphImage->w) && getpixel(glyphImage, x1, scanLine) != separatingColor) //find right edge of glyph
				{
					x1++;
				}
				
				int width = x1-x;
				int height = y1-scanLine;
				lastRowHeight = height;
				
				char currGlyph = aGlyphs.size() < currGlyphIndex ? specialGlyph++ : aGlyphs[currGlyphIndex++];
				BITMAP* bitmap = create_sub_bitmap(glyphImage, x, scanLine, width, height);
				myGlyphToBitmap[currGlyph] = bitmap;

				x = x1;
			}
			x += 1;
		}
		scanLine += lastRowHeight+1;
	}
	if(myGlyphToBitmap['\n'] == 0 && myGlyphToBitmap[' '] != 0) myGlyphToBitmap['\n'] = myGlyphToBitmap[' ']; //have something to draw when newline

	myHeight = lastRowHeight;
}

BITMAP* Font::getBitmapForGlyph(const char glyph)
{
	static BITMAP* defaultBitmap = myGlyphToBitmap[' ']; //TODO make bitmap
	BITMAP* bitmap = defaultBitmap;
	if(myGlyphToBitmap.find(glyph)!=myGlyphToBitmap.end())
	{
		bitmap = myGlyphToBitmap[glyph];
	}
	return bitmap;
}

void Font::drawWrap(BITMAP* aBuffer, const std::string& aText, int aX, int aY, int aMaxWidth, int aNumberOfCharacters)
{
	int x = aX;
	int y = aY;
	int glyphBitmapMaxHeight = 0;

	std::string text = aText;

	int totChar = 0;
	std::vector<std::string> words = tokenize(text, ' ');
	for(unsigned int i = 0; i < words.size(); i++)
	{
		std::string word = words[i];
		if(aMaxWidth != -1 && x + getWidth(word) > (unsigned int)aMaxWidth)
		{
			x = aX;
			y += glyphBitmapMaxHeight;
			glyphBitmapMaxHeight = 0;
		}
		else
		{
			if(totChar != 0) 
			{
				word = " " + word;
			}
		}

		for(unsigned int j = 0; j < word.size(); j++)
		{
			if(aNumberOfCharacters != -1 && totChar >= aNumberOfCharacters)
			{
				return;
			}
			char currChar = word[j];

			BITMAP* glyphBitmap = getBitmapForGlyph(currChar);

			glyphBitmapMaxHeight = max(glyphBitmapMaxHeight, glyphBitmap->h);

			drawGlyph(aBuffer, currChar, x, y);

			int glyphWidth = glyphBitmap->w;

			x += glyphWidth;
			if(currChar == '\n')
			{
				x = aX;
				y += glyphBitmapMaxHeight;
				glyphBitmapMaxHeight = 0;
			}
			totChar++;
		}
	}
}

unsigned int Font::getWidth(const std::string& aText)
{
	unsigned int width = 0;
	for(unsigned int i = 0; i < aText.size(); i++)
	{
		BITMAP* bitmap = getBitmapForGlyph(aText[i]);
		width += bitmap->w;
		
	}
	return width;
}

unsigned int Font::getHeight() const
{
	return myHeight;
}

void Font::drawGlyph(BITMAP* aBuffer, char aChar, int aX, int aY)
{
	BITMAP* glyphBitmap = getBitmapForGlyph(aChar);
	draw_sprite(aBuffer, glyphBitmap, aX, aY);
}

void Font::drawCenter( BITMAP* aBuffer, const std::string& aText, int aX, int aY, int aWidth, int aHeight)
{
	int x = aX + aWidth / 2 - getWidth(aText) / 2;
	int y = aY + aHeight / 2 - getHeight() / 2;

	for(unsigned int i = 0; i < aText.size(); i++)
	{
		BITMAP* bitmap = getBitmapForGlyph(aText[i]);
		draw_sprite(aBuffer, bitmap, x, y);
		x += bitmap->w;
	}
}

void Font::drawCenterY( BITMAP* aBuffer, const std::string& aText, int aX, int aY, int aHeight )
{
	int x = aX;
	int y = aY + aHeight / 2 - getHeight() / 2;

	for(unsigned int i = 0; i < aText.size(); i++)
	{
		BITMAP* bitmap = getBitmapForGlyph(aText[i]);
		draw_sprite(aBuffer, bitmap, x, y);
		x += bitmap->w;
	}
}

void Font::draw( BITMAP* aBuffer, const std::string& aText, int aX, int aY, Blending aBlending)
{
	int x = aX;
	int y = aY;

	bool addBlend = aBlending == Blending_Add;
	setBlending(aBlending);

	for(unsigned int i = 0; i < aText.size(); i++)
	{
		BITMAP* bitmap = getBitmapForGlyph(aText[i]);
		if (aBlending == Blending_None)
		{
			draw_sprite(aBuffer, bitmap, x, y);
		}
		else if (addBlend)
		{
			draw_trans_sprite(aBuffer, bitmap, x, y);
		}
		else
		{
			draw_lit_sprite(aBuffer, bitmap, x, y, 0);
		}
		x += bitmap->w;
	}
}

void Font::drawCenterYAlignRight( BITMAP* aBuffer, const std::string& aText, int aX, int aY, int aHeight )
{
	int x = aX - getWidth(aText);
	int y = aY + aHeight / 2 - getHeight() / 2;

	for(unsigned int i = 0; i < aText.size(); i++)
	{
		BITMAP* bitmap = getBitmapForGlyph(aText[i]);
		draw_sprite(aBuffer, bitmap, x, y);
		x += bitmap->w;
	}
}


