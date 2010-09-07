#pragma once

#include "ActionCast.hpp"

class ActionHack : public ActionCast {
public:
	ActionHack();

	void onPrepareCast();
	void onCastLogic();
	bool isCastDone();
private:
	int myTime;
	bool myIsDone;
};