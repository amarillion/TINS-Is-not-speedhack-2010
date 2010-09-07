#pragma once

#include "Action.hpp"
#include "Character.hpp"
#include "Animation.hpp"
#include "ActionCast.hpp"

class ActionIce2Spell : public ActionCast {
public:
	ActionIce2Spell();

	void onPrepareCast();
	void onCastLogic();
	bool isCastDone();

private:
	int myTargetIndex;
	int myCastTime;
};
