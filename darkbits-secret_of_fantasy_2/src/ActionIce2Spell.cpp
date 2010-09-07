#include "Precompiled.hpp"
#include "ActionIce2Spell.hpp"
#include "Character.hpp"
#include "Battle.hpp"
#include "Ice2Effect.hpp"

ActionIce2Spell::ActionIce2Spell()
	: ActionCast("Ice 2")
{
	
}

void ActionIce2Spell::onPrepareCast()
{
	getPerformer()->adjustMP(-getActionTrigger().getMPCost());
	myCastTime = 1111;
	myTargetIndex = -1;
}

void ActionIce2Spell::onCastLogic()
{
	myCastTime++;
	if (myCastTime > 40)
	{
		if (myTargetIndex >= 0)
		{
			Character* t = getTarget(myTargetIndex);
			t->damage(rand() % 100 + 30, Character::Damage_Ice);
		}

		myTargetIndex++;
		if (myTargetIndex == getNumTargets())
		{
			return;
		}
		myCastTime = 0;

		Character* t = getTarget(myTargetIndex);
		Ice2Effect* i = new Ice2Effect(t->getCenterX(), t->getCenterY(), t->getZOrder() + 0.01f);
		getBattle()->addEntity(i);
	}
}

bool ActionIce2Spell::isCastDone()
{
	return myTargetIndex == getNumTargets();
}
