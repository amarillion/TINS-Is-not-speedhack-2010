#include "Precompiled.hpp"
#include "EnemyCharacter.hpp"
#include "Battle.hpp"
#include "ActionFactory.hpp"
#include "DrawUtil.hpp"

EnemyCharacter::EnemyCharacter(const std::string &name, const std::string &sprite, int homeX, int homeY, int level, int hp, int mp, float speed )
	: Character(Faction_Enemy, name, homeX, homeY, level, hp, mp, speed)
	, myAnimation("data/images/monsters/" + sprite, 1)
	, myDeathTime( 0 )
	, myDeathIsDone( false )
	, myCastTime(1000)
{
	setHeight(myAnimation.getFrameHeight());
}

void EnemyCharacter::onRender( BITMAP* aBuffer)
{
	int x = getX() - myAnimation.getFrameWidth() / 2;
	int y = getY() - myAnimation.getFrameHeight() + 8;

	if (myDying)
	{
		float amount = (float)myDeathTime / myAnimation.getFrameHeight();
		myDeathIsDone = DrawUtil::dissolveBitmap(aBuffer, myAnimation.getFrame(0), x, y, amount);
	}
	else
	{
		Blending b = Blending_None;
		if (myCastTime < 20 && myCastTime % 5 < 2)
			b = Blending_Lighten;
		myAnimation.drawFrame(aBuffer, 0, x, y, false, false, b);
	}
	
// 	circle(aBuffer, getX(), getY(), 5, 0xffffffff);
// 	circle(aBuffer, getMarkerX(), getMarkerY(), 5, 0x88394829);
}

void EnemyCharacter::onLogic()
{
	Character::onLogic();

	myCastTime++;

	if (isDead()) {
		myDeathTime++;
		return;
	}

	if (!isReadyForAction()) {
		return;
	}
	
	std::vector<ActionTrigger> candidateTriggers;
	{
		std::set<ActionTrigger> knownTriggers = getActionTriggers();
		std::set<ActionTrigger>::const_iterator it;
		for (it = knownTriggers.begin(); it != knownTriggers.end(); ++it) {
			if (it->getMPCost() <= getMP()) {
				candidateTriggers.push_back(*it);
			}
		}
	}

	if (candidateTriggers.size() == 0) {
		return;
	}

	ActionTrigger chosenTrigger = candidateTriggers[rand() % candidateTriggers.size()];

	std::vector<Character *> candidateTargets;

	if (chosenTrigger.getAlignment() == ActionTrigger::Alignment_Defensive) {
		candidateTargets = myBattle->getEnemyCharacters();
	} else {
		candidateTargets = myBattle->getFightingPlayerCharacters();
	}

	{
		std::vector<Character *>::iterator it;
		for (it = candidateTargets.begin(); it != candidateTargets.end(); ) {
			if ((*it)->isDead() && chosenTrigger.getTargetPolicy() == ActionTrigger::TargetPolicy_OnlyAlive) {
				it = candidateTargets.erase(it);
			} else {
				++it;
			}
		}
	}

	if (chosenTrigger.getTargetMode() == ActionTrigger::TargetMode_None) {
		candidateTargets.clear();
	} else {
		if (candidateTargets.size() == 0) {
			return;
		}

		if (chosenTrigger.getTargetMode() == ActionTrigger::TargetMode_JustOne ||
			chosenTrigger.getTargetMode() == ActionTrigger::TargetMode_OneOrMany && (rand() & 1) == 0) {
			Character *target = candidateTargets[rand() % candidateTargets.size()];
			candidateTargets.clear();
			candidateTargets.push_back(target);
		}
	}
	
	myBattle->enqueueAction(ActionFactory::createAction(chosenTrigger, this, candidateTargets));
}

void EnemyCharacter::enterDeadState()
{
	Character::enterDeadState();
	myDeathTime = 0;
}

bool EnemyCharacter::isDeadStateEntered()
{
	return isDead() && myDeathIsDone;
}

void EnemyCharacter::enterCastState()
{
	myCastTime = 0;
}

void EnemyCharacter::enterAttackState()
{
	myCastTime = 0;
}

void EnemyCharacter::flee()
{
	setHP(0);
}
