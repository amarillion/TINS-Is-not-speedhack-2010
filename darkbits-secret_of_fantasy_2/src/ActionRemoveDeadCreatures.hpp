#pragma once

#include "Action.hpp"

class ActionRemoveDeadCreatures : public Action {
public:
	ActionRemoveDeadCreatures();

	virtual ~ActionRemoveDeadCreatures();

	virtual void onEnter(Battle *battle);

	virtual void onLeave();

	virtual void onLogic();

	virtual bool isDone();

private:
	std::vector<Character *> myDyingCharacters;
	bool myDone;
};