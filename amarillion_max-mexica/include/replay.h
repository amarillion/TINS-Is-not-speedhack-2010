#ifndef REPLAY_H
#define REPLAY_H

#include <vector>
#include "button.h"
#include "settings.h"
#include <assert.h>
#include <time.h>
#include <stdlib.h>

struct ReplayBit
{
	bool state[NUM_BUTTONS];
	int repeat;
};

class IReplay
{
public:
	virtual bool getState(int i) = 0;
	virtual bool update() = 0;
};

class ReplayRecorder : public IReplay
{
	int seed;
	std::vector<ReplayBit> bits;
	std::vector<ReplayBit>::iterator pos;

	Button *regulars;

public:

	ReplayRecorder(Button *_regulars) : bits(), regulars (_regulars)
	{
		assert (regulars);

		srand(time(NULL));
		seed = rand();
		srand(seed);
	}

	void writeToFile(int i);

	virtual bool update();

	virtual bool getState (int i);
};

class ReplayPlayer : public IReplay
{
private:
	void readFromFile(int i);
	int seed;
	int repeat;
	std::vector<ReplayBit> bits;
	std::vector<ReplayBit>::iterator pos;
public:
	Button *regulars;

	ReplayPlayer (Button *_regulars, int index) : regulars (_regulars)
	{
		readFromFile (index);
		pos = bits.begin();
		repeat = (*pos).repeat;
		srand(seed);
	}

	virtual bool getState (int i);
	virtual bool update();
};

bool replayFileExists(int index);

#endif
