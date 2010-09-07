#include "Precompiled.hpp"
#include "ActionCast.hpp"
#include "Battle.hpp"
#include "Gui.hpp"

ActionCast::ActionCast( const std::string& name )
:myName(name)
,myBattle(0)
,myTime(0)
{

}

void ActionCast::onEnter( Battle *battle )
{
	myBattle = battle;
	myState = State_New;

}

void ActionCast::onLeave()
{

}

void ActionCast::onLogic()
{
	if (myState == State_Done)
		return;

	if (myState == State_New)
	{
		myTime++;
		if (myTime > 30)
		{
			myTime = 0;
			getPerformer()->enterCastState();
			
			myState = State_PreWait;
		}
	}

	if (myState == State_PreWait)
	{
		myTime++;
		if (myTime > 30)
		{
			myBattle->getGui()->showMessage(myName);
			myState = State_ShowMessage;
			myTime = 0;
		}
	}

	if (myState == State_ShowMessage)
	{
		myTime++;

		if (myTime > 30 && getPerformer()->isCastStateEntered())
		{
			myTime = 0;
			myState = State_Cast;
			onPrepareCast();
		}
	}

	if (myState == State_Cast)
	{
		if (isCastDone())
		{
			getPerformer()->enterIdleState();
			myState = State_Done;
		}
		else
		{
			onCastLogic();
		}
	}
}

bool ActionCast::isDone()
{
	return myState == State_Done;
}

Battle* ActionCast::getBattle()
{
	return myBattle;
}
