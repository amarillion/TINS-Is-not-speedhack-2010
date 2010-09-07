#include "Precompiled.hpp"
#include "ActionHack.hpp"
#include "HackEntity.hpp"
#include "Battle.hpp"

ActionHack::ActionHack()
:ActionCast("Hack")
,myIsDone(false)
{

}

void ActionHack::onPrepareCast()
{
	myTime = 0;
	getBattle()->addEntity(new HackEntity(120));
}

void ActionHack::onCastLogic()
{
	if (myTime > 120)
	{
		int t = getNumTargets();
		for (int i = 0; i < getNumTargets(); i++) {
			getTarget(i)->damage(30 + rand() % 40, Character::Damage_Hack);
		}	
		myIsDone = true;
	}

	myTime++;
}

bool ActionHack::isCastDone()
{
	return myIsDone;
}
