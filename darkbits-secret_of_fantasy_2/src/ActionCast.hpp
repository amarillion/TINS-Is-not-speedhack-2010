#pragma once

#include "Action.hpp"
#include "Character.hpp"
#include "Animation.hpp"

class ActionCast : public Action {
public:
	ActionCast(const std::string& name);

	void onEnter(Battle *battle);

	void onLeave();

	void onLogic();

	bool isDone();

	virtual void onPrepareCast() = 0;
	virtual void onCastLogic() = 0;
	virtual bool isCastDone() = 0;

	Battle* getBattle();

private:
	std::string myName;
	Battle *myBattle;
	int myTime;

	enum State
	{
		State_New,
		State_PreWait,
		State_ShowMessage,
		State_Cast,
		State_Done
	};
	State myState;
};
