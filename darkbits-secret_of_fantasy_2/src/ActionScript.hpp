#pragma once
#include "Action.hpp"
class Battle;
class ActionScript : public Action {
public:
	ActionScript( void aScript(Battle *aBattle));
	
	virtual void onEnter(Battle *battle);

	virtual void onLeave();

	virtual void onLogic();

	virtual bool isDone();
private:
	void(*myScript)(Battle *aBattle);
	Battle *myBattle;
};
