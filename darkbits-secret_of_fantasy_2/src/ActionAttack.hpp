#pragma once

#include "Action.hpp"
#include "Animation.hpp"

class ActionAttack : public Action {
public:
	ActionAttack();

	virtual void onEnter(Battle *battle);

	virtual void onLeave();

	virtual void onLogic();

	virtual bool isDone();

private:
	enum State
	{
		State_InitalWait,
		State_PreWait,
		State_Attack,
		State_PostWait,
		State_Done
	};
	State myState;
	int myTime;
	Animation myAnimation;
	Battle* myBattle;
};