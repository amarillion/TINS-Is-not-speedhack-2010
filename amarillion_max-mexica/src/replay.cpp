#include "replay.h"
#include <allegro.h>
#include <iostream>
#include <fstream>

using namespace std;

const int SLOT_NUM=5;
const char *SLOTS[SLOT_NUM] = { "1.rpl", "2.rpl", "3.rpl", "4.rpl", "5.rpl" };

void ReplayRecorder::writeToFile(int i)
{
	ofstream f;
	f.open (SLOTS[i], ios::out);
	f << seed << endl;
	f << bits.size() << endl;
	for (vector<ReplayBit>::iterator i = bits.begin(); i != bits.end(); ++i)
	{
		int stateVal = 0;
		for (int j = 0; j < NUM_BUTTONS; ++j)
		{
			stateVal <<= 1;
			stateVal += (*i).state[j] ? 1 : 0;
		}
		f << stateVal << endl;
		f << (*i).repeat << endl;
	}
	f.close();
}

void ReplayPlayer::readFromFile(int i)
{
	ifstream f;
	f.open (SLOTS[i], ios::in);
	f >> seed;
	int size;
	f >> size;
	for (int i = 0; i < size; ++i)
	{
		ReplayBit bit;

		int stateVal;
		f >> stateVal;
		for (int j = NUM_BUTTONS-1; j >= 0; --j)
		{
			bit.state[j] = (stateVal & 1);
			stateVal >>= 1;
		}
		f >> bit.repeat;
		bits.push_back(bit);
	}
	f.close();
}

bool ReplayRecorder::update()
{
	ReplayBit nextBit;
	nextBit.repeat = 0;

	bool same = false;

	for (int i = 0; i < NUM_BUTTONS; ++i)
	{
		nextBit.state[i] = regulars[i].getState();
	}

	if (bits.size() > 0)
	{
		same = true;
		vector<ReplayBit>::iterator current = bits.end() - 1;
		for (int i = 0; i < NUM_BUTTONS; ++i)
		{
			if (nextBit.state[i] != (*current).state[i])
				same = false;
		}
		if (same)
		{
			current->repeat++;
		}
	}

	if (!same)
		bits.push_back(nextBit);

	return true;
}

bool replayFileExists(int index)
{
	return file_exists(SLOTS[index], 0, NULL);
}

bool ReplayRecorder::getState (int i)
{
	if (bits.size() > 0)
	{
		vector<ReplayBit>::iterator current = bits.end() - 1;
		return (*current).state[i];
	}
	else
	{
		return false;
	}
}

bool ReplayPlayer::getState (int i)
{
	return (*pos).state[i];
}

bool ReplayPlayer::update()
{
	repeat--;
	if (repeat < 0)
	{
		pos++;
		repeat = (*pos).repeat;
	}

	return !(pos == bits.end());
}
