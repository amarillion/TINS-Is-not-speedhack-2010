#pragma once

#include "Action.hpp"

class Character;
class Battle;
class ActionIntroduceCharacter: public Action
{
public:
	ActionIntroduceCharacter(Character *aCharacter);
	void onEnter(Battle *battle);
	void onLeave();
	void onLogic();
	bool isDone();

private:
	Character *myCharacter;
	Battle *myBattle;
	bool myIsDone;
};