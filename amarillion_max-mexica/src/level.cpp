#include "level.h"
#include "stdlib.h"
#include "assert.h"
#include <iostream>

using namespace xdom;
using namespace std;

int LevelData::readIntSafe(DomNode &n, const char *name)
{
	assert (n.attributes.find (name) != n.attributes.end());
	return atoi(n.attributes[name].c_str());
}

LevelData LevelData::loadFromXml(DomNode &n)
{
	LevelData result;

	assert (n.name == "level");

	result.foodMsecDev = readIntSafe (n, "foodmsecdev");
	assert (result.foodMsecDev > 0);

	result.foodMsecMin = readIntSafe (n, "foodmsecmin");
	result.foodScore = readIntSafe (n, "foodscore");

	result.foodType = readIntSafe (n, "foodtype");
	result.pepperNum = readIntSafe (n, "peppernum");
	assert (result.pepperNum > 0);

	result.pepperMsecDev = readIntSafe (n, "peppermsecdev");
	assert (result.pepperMsecDev > 0);
	result.pepperMsecMin = readIntSafe (n, "peppermsecmin");
	result.pepperType = readIntSafe (n, "peppertype");

	return result;

}
