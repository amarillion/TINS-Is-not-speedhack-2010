#ifndef ANIM_H
#define ANIM_H

#include "resources.h"
#include <stdio.h>
#include <vector>
#include <map>
#include <string>
#include <assert.h>
#include "dom.h"

class DirectionModel
{
	private:
		int numDirections;
		const char ** directions;
	public:
		DirectionModel (const char * _directions[], int _numDirections)
		{
			numDirections = _numDirections;
			directions = _directions;
		}

		int getNumDirections() { return numDirections; }
		int idToIndex(std::string id)
		{
			for (int i = 0; i < numDirections; ++i)
			{
				if (id == directions[i]) return i;
			}
			return -1;
		}
};


class CompositePart
{
friend class Frame;
friend class Anim;
private:
	RLE_SPRITE *rle;
	int hotx;
	int hoty;
public:
	CompositePart () {}
	CompositePart (RLE_SPRITE *_rle, int _hotx = 0, int _hoty = 0) :
		rle (_rle), hotx (_hotx), hoty (_hoty) {}
	RLE_SPRITE *getRle() { return rle; }
};

/* 
	cumulative: end of frame time, cumulative, so that frames[0].cumulative == frames[0].length
		and frames[frames.size()-1].cumulative == totalLength
*/
class Frame
{
friend class Sequence;
friend class Anim;
private:
	int length;
	int cumulative; 
	std::vector<CompositePart> parts;
public:
	Frame () : length (0), cumulative (0), parts() {}
	Frame (CompositePart _onlyPart, int _length = 0) :
		length (_length), cumulative(0), parts() { parts.push_back (_onlyPart); }
	void draw(BITMAP *buf, int x, int y) const;
	CompositePart &getPart(int i = 0) { return parts[i]; }
	void getCompositeBounds (int &sprx, int &spry, int &width, int &height) const;
};

class Sequence
{
friend class Anim;
private:
	std::vector <Frame> frames;
	int totalLength;
	bool loop; //TODO: send event at end of non-loop
public:
	Sequence() : frames(), totalLength(0), loop(true) {}
	const Frame *getFrame (int time) const;
	void add (Frame &f);
};

/**
	Anim consists of one or more states
	states consist of one or more dirs
	dir consists of one or more frames
	frames consist of one or more parts

	There may be 1, 2, 4 or 8 dirs for a sprite
	The dir you ask for will be normalized to the number of dirs available.
*/
class Anim
{
private:
	// states	// dirs		  
	std::vector < std::vector < Sequence > > frames;
	
	static std::vector < Sequence > loadState (xdom::DomNode &i, Resources *res);
	static Sequence loadSequence (xdom::DomNode &i, Resources *res);
	static Frame loadFrame (xdom::DomNode &i, Resources *res);
	static CompositePart loadPart (xdom::DomNode &i, Resources *res);
	static RLE_SPRITE *getLitRle(std::string id, int r, int g, int b, int a, Resources *resources);

	static DirectionModel model;
public:
	int sizex;
	int sizey;
	int sizez;
	
	Anim();
	~Anim() {}
	
	static void loadFromXml (xdom::DomNode &n, Resources *res, std::map<std::string, Anim*> &result);	
 	void drawFrame (BITMAP *buf, int state, int dir, int time, int x, int y) const;
 	int getNumStates () const { return frames.size(); }

 	int getTotalDirs (int state = 0) { return frames[state].size(); }
 	int getTotalLength (int state, int dir) { return frames[state][dir].totalLength; }

 	const Frame *getFrame (int state, int dir, int time) const;

	void add (Frame &f, int state = 0, int dir = 0);

 	static void setDirectionModel (DirectionModel value)
 	{
 		model = value;
 	}
};

/**
	holds a combination of Anim, state and dir
*/
class AnimRunner
{
public:
	Anim *anim;
	int state;
	int dir;
	int startTime;
	
	void drawFrame (BITMAP *buf, int x, int y, int currentTime) const;
};

#endif
