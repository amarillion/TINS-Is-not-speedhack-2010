#include "Precompiled.hpp"
#include "ActionSleep.hpp"

ActionSleep::ActionSleep( int sleep )
:mySleep(sleep)
{

}

bool ActionSleep::isDone()
{
	return mySleep < 0;
}

void ActionSleep::onLogic()
{
	mySleep--;
}

void ActionSleep::onLeave()
{

}

void ActionSleep::onEnter( Battle *battle )
{

}


