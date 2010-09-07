#include "Precompiled.hpp"
#include "ActionScript.hpp"
#include "Battle.hpp"

ActionScript::ActionScript( void aScript(Battle *aBattle) )
: myScript(aScript)
, myBattle(NULL)
{

}

void ActionScript::onEnter( Battle *battle )
{
	myBattle = battle;
	myScript(battle);
}

void ActionScript::onLeave()
{

}

void ActionScript::onLogic()
{

}

bool ActionScript::isDone()
{
	return myBattle != NULL;
}