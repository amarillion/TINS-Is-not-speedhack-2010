#include <allegro.h>
#include <map>
#include <string>
#include <vector>
#include "anim.h"
#include <alfont.h>
#include "level.h"

#ifndef RESOURCES_H
#define RESOURCES_H

class Anim;
	
class Resources
{
	std::vector<DATAFILE*> data;
	std::map<std::string, RLE_SPRITE*> sprites; // seperately loaded bitmaps	
	std::map <std::string, Anim*> animlist;

	std::vector <RLE_SPRITE*> generated; // run-time generated RLE's
	std::map <std::string, ALFONT_FONT *> alfonts;
	std::map <std::string, SAMPLE *> samples;
	
	std::vector<LevelData> levelData;
// 	void makeHitAnim (const std::string &id, const std::string &id2);
// 	void makeOutlineAnim (const std::string &srcName, const std::string &destName);
public:
	int init(); // loads resources
	~Resources();
	
	std::map <std::string, Anim*> getAnimList() { return animlist; }
	Anim *getAnim (std::string id);
	FONT *getFont (std::string id);
	BITMAP *getBitmap (std::string id);
	RLE_SPRITE *getRle (std::string id);
	SAMPLE *getSample (std::string id);  
	ALFONT_FONT *getAlfont (std::string id);
	void putRle(std::string id, RLE_SPRITE *rle);

	LevelData getLevelData(int i) { return levelData[i % levelData.size()]; }
};

void glob(const char *pattern, std::vector<std::string> &result);

#endif
