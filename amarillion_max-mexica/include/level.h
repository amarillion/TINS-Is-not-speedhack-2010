#ifndef LEVEL_H
#define LEVEL_H

#include "dom.h"

class LevelData
{
public:
	int pepperNum;
	int pepperType;
	int pepperMsecMin;
	int pepperMsecDev;
	int foodType;
	int foodMsecMin;
	int foodMsecDev;
	int foodScore;

	LevelData() : pepperNum(2), pepperType(0),
			pepperMsecMin(500),
			pepperMsecDev(500),
			foodType(0),
			foodMsecMin(2000),
			foodMsecDev(2000),
				foodScore(100)
	{
	}

	static LevelData loadFromXml(xdom::DomNode &n);
private:
	static int readIntSafe(xdom::DomNode &n, const char *name);
};

#endif
