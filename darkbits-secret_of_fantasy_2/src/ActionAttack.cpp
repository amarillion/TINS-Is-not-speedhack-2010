#include "Precompiled.hpp"
#include "ActionAttack.hpp"
#include "Character.hpp"
#include "ParticleEntity.hpp"
#include "Battle.hpp"
#include "Sound.hpp"

ActionAttack::ActionAttack()
:myState(State_InitalWait)
,myTime(0)
,myAnimation("data/images/attack.bmp")
{

}

void ActionAttack::onEnter(Battle *battle)
{
	myBattle = battle;	
}

void ActionAttack::onLeave()
{
}

void ActionAttack::onLogic()
{
	if (myState == State_Done)
		return;

	if (myState == State_InitalWait)
	{
		myTime++;
		if (myTime > 20)
		{
			getPerformer()->enterAttackState();
			myState = State_PreWait;
			myTime = 0;
		}
	}

	if (myState == State_PreWait)
	{
		myTime++;
		if (myTime > 30)
		{
			myState = State_Attack;
		}
	}

	if (myState == State_Attack)
	{

		if (getPerformer()->isAttackStateEntered())
		{
			for (int i = 0; i < getNumTargets(); i++)
			{
				getTarget(i)->damage(getPerformer()->getLevel() * 10, Character::Damage_Physical);
				ParticleEntity* pe =new ParticleEntity("data/images/attack.bmp", 4, 1.0f, getTarget(i)->getCenterX(), getTarget(i)->getCenterY(), getTarget(i)->getZOrder() + 0.01f);
				myBattle->addEntity(pe);
				Sound::playSample("data/sound/attack.wav");
			}
			myState = State_PostWait;
			myTime = 0;
		}
	}
	if (myState == State_PostWait)
	{
		myTime++;
		if (myTime > 30)
		{
			getPerformer()->enterIdleState();
			myState = State_Done;
		}
	}
}

bool ActionAttack::isDone()
{
	return myState == State_Done;
}

