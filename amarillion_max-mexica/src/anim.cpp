#include <assert.h>
#include "anim.h"
#include "color.h"

using namespace xdom;
using namespace std;

const char * defaultdirections[] = { "n", "e", "s", "w" };

DirectionModel Anim::model(defaultdirections, 4);

Anim::Anim() : frames(), sizex (0), sizey (0), sizez (0)
{
}

void Anim::loadFromXml (DomNode &n, Resources *res, map<string, Anim*> &result)
{
	assert (n.name == "anim");
	Anim *temp;
	string name = n.attributes["id"];
	vector<DomNode>::iterator h;
	
	temp = new Anim;
	temp->sizex = atoi (n.attributes["dx"].c_str());
	temp->sizey = atoi (n.attributes["dy"].c_str());
	temp->sizez = atoi (n.attributes["dz"].c_str());
	
	h = n.children.begin();
	if (h->name == "state")
	{
		for (; h != n.children.end(); ++h)
		{
			temp->frames.push_back (loadState (*h, res));
		}
	}
	else
	{
		temp->frames.push_back (loadState (n, res));
	}
	
	result.insert (pair<string, Anim*>(name, temp));
}

Sequence Anim::loadSequence (xdom::DomNode &i, Resources *res)
{
	Sequence result;
	result.totalLength = 0;
	result.loop = true;
		
	vector<DomNode>::iterator j;
	for (j = i.children.begin(); j != i.children.end(); ++j)
	{
		Frame f = loadFrame (*j, res);
		result.add (f);
	}
	return result;
}

vector < Sequence > Anim::loadState (DomNode &i, Resources *res)
{
	vector < Sequence > result;
	
	vector<DomNode>::iterator h;

	Sequence temp [model.getNumDirections()];
	int totalDirs = 0;
	
	h = i.children.begin();
	
	if (h->name == "frame" || h->name == "composite")
	{
		temp[0] = loadSequence (i, res);
		totalDirs = 1;
	}
	else if (h->name == "dir")
	{	
		for (; h != i.children.end(); ++h)
		{
			int length = 0;	
			string id = h->attributes["id"];
			int dir = model.idToIndex(id);
			if (dir < 0)
			{
				allegro_message ("Unknown direction %s", id.c_str());
				assert (false); // unknown tag
			}
			else
			{
				totalDirs++;
			}
			temp[dir] = loadSequence (*h, res);
		}
	}
	else
	{
		allegro_message ("Unknown tag %s", h->name.c_str());
		assert (false);
	}
	assert (totalDirs == 1 || totalDirs == model.getNumDirections());
	// either "all"  (meaning all the same) or four dirs
	
	for (int k = 0; k < totalDirs; ++k)
	{
		for (int l = 0; l < (model.getNumDirections() / totalDirs); ++l)
		{
			result.push_back (temp[k]);
		}
	}
	
	return result;
}

void Frame::getCompositeBounds (int &sprx, int &spry, int &width, int &height) const
{
	vector<CompositePart>::const_iterator i;
	i = parts.begin();
	int x1 = i->hotx;
	int y1 = i->hoty;
	int x2 = i->hotx + i->rle->w;
	int y2 = i->hoty + i->rle->h;
	++i;
	// now grow those bounds if any of the next parts fall outside it
	for (; i != parts.end(); ++i)
	{
		if (i->hotx < x1) x1 = i->hotx;
		if (i->hoty < y1) y1 = i->hoty;
		if (i->hotx + i->rle->w > x2) x2 = i->hotx + i->rle->w;
		if (i->hoty + i->rle->h > y2) y2 = i->hoty + i->rle->h;
	}
	sprx = x1;
	spry = y1;
	width = x2 - x1;
	height = y2 - y1;
}

Frame Anim::loadFrame (DomNode &i, Resources *res)
{
	Frame result;
	vector<DomNode>::iterator j;
	
	int time = 0;
	if (i.attributes.find ("time") != i.attributes.end())
		time = atoi(i.attributes["time"].c_str());
	result.length = time;
	if (i.name == "frame")
	{
		// just a single part.
		CompositePart part = loadPart (i, res);
		result.parts.push_back (part);
	}
	else if (i.name == "composite")
	{
		for (j = i.children.begin(); j != i.children.end(); ++j)
		{
			// multiple parts
			CompositePart part = loadPart (*j, res);
			result.parts.push_back (part);
		}
	}
	else
	{
		assert (false); // unknown tag
	}
	
	return result;
}

RLE_SPRITE *Anim::getLitRle(string id, int r, int g, int b, int a, Resources *res)
{
	RLE_SPRITE * s = res->getRle (id);;
	BITMAP *temp = create_bitmap (s->w, s->h);
	clear_to_color (temp, MAGIC_PINK);
	draw_rle_sprite (temp, s, 0, 0);
	set_trans_blender (r, g, b, 255);
	draw_lit_rle_sprite (temp, s, 0, 0, a);
	RLE_SPRITE *temp_rle = get_rle_sprite (temp);
	destroy_bitmap (temp);
	res->putRle (id + "lit", temp_rle);
	return temp_rle;
}

CompositePart Anim::loadPart (DomNode &i, Resources *res)
{
	CompositePart result;
	
	result.hotx = atoi(i.attributes["hotx"].c_str());
	result.hoty = atoi(i.attributes["hoty"].c_str());

	// handle "lit" effect
	if (i.children.size() > 0 && i.children[0].name == "lit")
	{
		DomNode &j = i.children[0];
		int r = atoi (j.attributes["r"].c_str());
		int g = atoi (j.attributes["g"].c_str());
		int b = atoi (j.attributes["b"].c_str());
		int a = atoi (j.attributes["alpha"].c_str());
		result.rle = getLitRle (i.attributes["rle"], r, g, b, a, res);
	}
	else
	{
		result.rle = res->getRle (i.attributes["rle"]);
	}

	if (!result.rle)
	{
		allegro_message ("error loading animation frame");
	}
	return result;
}

void Frame::draw(BITMAP * buf, int x, int y) const
{
	vector<CompositePart>::const_iterator i;
	for (i = parts.begin(); i != parts.end(); ++i)
	{
		draw_rle_sprite (buf, i->rle, x + i->hotx, y + i->hoty);
	}
}

const Frame *Sequence::getFrame (int time) const
{
	if (loop && totalLength > 0)
	{
		time %= totalLength;
	}
	
	int i = 0;
	while ((i < frames.size()) && (time > frames[i].cumulative)) { i++; }
	if (i == frames.size()) i--;
	
	return &frames[i];
}

const Frame *Anim::getFrame (int state, int dir, int time) const
{
	assert (state >= 0 && state <= frames.size());
	assert (dir >= 0 && dir <= frames[state].size());
	const Sequence &s = frames[state][dir];
	return s.getFrame (time);
}

void Anim::drawFrame (BITMAP *buf, int state, int dir, int time, int x, int y) const
{
	const Frame *current = getFrame (state, dir, time);
	current->draw (buf, x, y);
}

void Sequence::add (Frame &f)
{
	totalLength += f.length;
	f.cumulative = totalLength;
	frames.push_back (f);
}

void Anim::add (Frame &f, int state, int dir)
{
	if (state >= frames.size()) frames.resize(state + 1);
	if (dir >= frames[state].size()) frames[state].resize(dir + 1);
	frames[state][dir].add(f);
}
