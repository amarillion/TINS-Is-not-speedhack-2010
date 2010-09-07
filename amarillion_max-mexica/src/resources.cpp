#include <assert.h>
#include "resources.h"
#include "color.h"
#include "dom.h"
#include <logg.h>

using namespace std;
using namespace xdom;

#ifndef PI
#define PI 3.1415926
#endif

// adds all filenames that match pattern to the vector
void glob(const char *pattern, vector<string> &result)
{
	char buf[1024];
	
	al_ffblk info;
	
	int temp = al_findfirst (pattern, &info, FA_RDONLY | FA_ARCH);
	
	while (temp == 0)
	{
		replace_filename(buf, pattern, info.name, sizeof(buf));
		result.push_back (std::string(buf));
		temp = al_findnext(&info);
	}
			
	al_findclose(&info);
}

int Resources::init()
// returns 1 on succes, 0 on failure
{
	
	{
		// load all .ttf files in data directory
		vector <string> fontfilenames;
		glob ("data/*.ttf", fontfilenames);
		for (vector<string>::iterator i = fontfilenames.begin();
			i != fontfilenames.end(); ++i)
		{
			unsigned int start = i->find_last_of ("\\/");
			if (start == string::npos) start = 0;
			else start += 1;
				
			unsigned int end = i->rfind (".");
			if (end == string::npos) end = i->size();
			else end -= 1;
			
			// generate fonts with sizes 8, 16, 24, etc. to 48
			for (int k = 8; k < 48; k += 8)
			{
				ALFONT_FONT *temp;
				if (!(temp = alfont_load_font (i->c_str())))
				{
					return 0;
				} else {
					char namebuf[128];
					snprintf(namebuf, sizeof(namebuf), "%s_%i", i->substr(start, end-start+1).c_str(), k);
					string name = string(namebuf);
					alfont_set_font_size (temp, k);
					alfonts.insert (pair<string, ALFONT_FONT*>(name, temp));
				}
			}

		}
	}
	vector <string> bmpfilenames;	
	glob ("data/*.bmp", bmpfilenames);
	
	for (vector<string>::iterator i = bmpfilenames.begin();
		i != bmpfilenames.end(); ++i)
	{
		unsigned int start = i->find_last_of ("\\/");
		if (start == string::npos) start = 0;
		else start += 1;
			
		unsigned int end = i->rfind (".");
		if (end == string::npos) end = i->size();
		else end -= 1;
			
		BITMAP *bmp;
		bmp = load_bitmap (i->c_str(), NULL);
		if (!bmp)
		{
			return 0;
		}
		else
		{
			string name = i->substr(start, end-start+1);
			sprites.insert (pair<string, RLE_SPRITE*>(name, get_rle_sprite(bmp)));
			destroy_bitmap (bmp);
		}
	}

	vector <string> oggfilenames;
	glob ("data/*.ogg", oggfilenames);

	for (vector<string>::iterator i = oggfilenames.begin();
		i != oggfilenames.end(); ++i)
	{
		unsigned int start = i->find_last_of ("\\/");
		if (start == string::npos) start = 0;
		else start += 1;

		unsigned int end = i->rfind (".");
		if (end == string::npos) end = i->size();
		else end -= 1;

		SAMPLE *s;
		s = logg_load(i->c_str());
		if (!s)
		{
			return 0;
		}
		else
		{
			string name = i->substr(start, end-start+1);
			samples.insert (pair<string, SAMPLE*>(name, s));
		}
	}

	// load xml data	
	vector <string> xmlfilenames;	
	glob ("data/*.xml", xmlfilenames);

	for (vector <string>::iterator i = xmlfilenames.begin(); i != xmlfilenames.end(); ++i)
	{
		DomNode node = xmlParseFile(*i);
		
		vector<DomNode>::iterator n;
		for (n = node.children.begin(); n != node.children.end(); ++n)
		{
			if (n->name == "anim")
			{
				Anim::loadFromXml (*n, this, animlist);
			}
			else if (n->name == "level")
			{
				levelData.push_back (LevelData::loadFromXml (*n));
			}
			else
			{
				assert (false); // unknown tag
			}
		}
	}

	return 1;
}

Resources::~Resources()
{
	{
		map<string, RLE_SPRITE*>::iterator i;
		for (i = sprites.begin(); i != sprites.end(); i++)
		{
			destroy_rle_sprite (i->second);
		}
	}
	{
		map<string, Anim*>::iterator i;
		for (i = animlist.begin(); i != animlist.end(); i++)
		{
			delete (i->second);
		}
	}
	{
		map<string, ALFONT_FONT*>::iterator i;
		for (i = alfonts.begin(); i != alfonts.end(); i++)
		{
			alfont_destroy_font (i->second);
		}
	}
}

BITMAP *Resources::getBitmap (string id)
{
	vector<DATAFILE*>::iterator i;
	DATAFILE *temp = NULL;
	for (i = data.begin(); i != data.end() && temp == NULL; ++i)
	{
			assert (*i);
			temp = find_datafile_object (*i, id.c_str());				
			// check if object is of the right type
			if (temp && temp->type != DAT_BITMAP)
					temp = NULL;
	}
	if (temp)
		return (BITMAP*)temp->dat;
	else
	{
		allegro_message ("Couldn't find bitmap '%s'", id.c_str());			
		exit(1);
	}
	return NULL;
}

FONT *Resources::getFont (string id)
{
	vector<DATAFILE*>::iterator i;
	DATAFILE *temp = NULL;
	for (i = data.begin(); i != data.end() && temp == NULL; ++i)
	{
			assert (*i);
			temp = find_datafile_object (*i, id.c_str());				
			// check if object is of the right type
			if (temp && temp->type != DAT_FONT)
					temp = NULL;
	}
	if (temp)
		return (FONT*)temp->dat;
	else
	{
		allegro_message ("Couldn't find FONT '%s'", id.c_str());			
		exit(1);
	}
	return NULL;	
}

RLE_SPRITE *Resources::getRle (string id)
{
	vector<DATAFILE*>::iterator i;
	DATAFILE *temp = NULL;
	for (i = data.begin(); i != data.end() && temp == NULL; ++i)
	{
			assert (*i);
			temp = find_datafile_object (*i, id.c_str());				
			// check if object is of the right type
			if (temp && temp->type != DAT_RLE_SPRITE)
					temp = NULL;
	}
	
	if (temp)
			return (RLE_SPRITE*)temp->dat;
	else
	{
		if (sprites.find (id) == sprites.end())
		{
			allegro_message ("Couldn't find RLE sprite '%s'", id.c_str());			
			exit(1);
		}
		
		return sprites[id];
	}
	
	assert (false);
	return NULL;
}

SAMPLE *Resources::getSample (string id)
{
	vector<DATAFILE*>::iterator i;
	DATAFILE *temp = NULL;
	for (i = data.begin(); i != data.end() && temp == NULL; ++i)
	{
			assert (*i);
			temp = find_datafile_object (*i, id.c_str());				
			// check if object is of the right type
			if (temp && temp->type != DAT_SAMPLE)
					temp = NULL;
	}
	if (temp)
			return (SAMPLE*)temp->dat;
	else
	{
		if (samples.find (id) == samples.end())
		{
			allegro_message ("Couldn't find RLE sprite '%s'", id.c_str());
			exit(1);
		}

		return samples[id];
	}
}

ALFONT_FONT *Resources::getAlfont (string id)
{
	if (alfonts.find (id) == alfonts.end())
	{
		allegro_message ("Couldn't find ttf font '%s'", id.c_str());			
		exit(1);
	}
	else
	{
		return alfonts[id];
	}
}

Anim *Resources::getAnim (string id)
{
	if (animlist.find (id) == animlist.end())
	{
		allegro_message ("Couldn't find Animation '%s'", id.c_str());			
		exit(1);
	}
	else
	{
		return animlist[id];
	}
}

void Resources::putRle(std::string id, RLE_SPRITE *rle)
{
	sprites.insert (pair<string, RLE_SPRITE *>(id, rle));
}
