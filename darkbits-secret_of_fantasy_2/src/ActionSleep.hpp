#pragma once

#include "Action.hpp"

class ActionSleep: public Action
{
public:
	ActionSleep(int sleep);

	void onEnter(Battle *battle);

	void onLeave();

	void onLogic();

	bool isDone();
private:
	int mySleep;
};