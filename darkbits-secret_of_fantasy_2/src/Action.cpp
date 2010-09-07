#include "Precompiled.hpp"
#include "Action.hpp"
#include "EnemyCharacter.hpp"
#include "PlayerCharacter.hpp"

Action::Action()
	: myPerformer(NULL)
	, myHadSingleTarget(true)
	, myTargetFaction(Character::Faction_None)
	, myCancelled(false)
{
}

Action::~Action()
{
}

const std::string &Action::getName()
{
	return myTrigger.getName();
}

Character *Action::getPerformer()
{
	return myPerformer;
}

int Action::getNumTargets()
{
	return myTargets.size();
}

Character *Action::getTarget(int i)
{
	return myTargets.at(i);
}

void Action::setPerformer( Character *performer )
{
	myPerformer = performer;
}

void Action::setTargets( const std::vector<Character *> &targets )
{
	myTargets = targets;
	myHadSingleTarget = targets.size() == 1;
	if (targets.size() > 0) {
		myTargetFaction = targets[0]->getFaction();
	}
}

void Action::removeTarget( Character *target )
{
	std::vector<Character *>::iterator it;
	for (it = myTargets.begin(); it != myTargets.end(); ++it) {
		if (*it == target) {
			myTargets.erase(it);
			return;
		}
	}
}

void Action::setActionTrigger( const ActionTrigger &trigger )
{
	myTrigger = trigger;
}

const ActionTrigger & Action::getActionTrigger()
{
	return myTrigger;
}

bool Action::hadSingleTarget()
{
	return myHadSingleTarget;
}

void Action::cancel()
{
	myCancelled = true;
}

bool Action::isCancelled()
{
	return myCancelled;
}

bool Action::isTargetOfAction(Character *character)
{
	for (int i = 0; i < getNumTargets(); i++) {
		if (getTarget(i) == character) {
			return true;
		}
	}
	return false;
}

Character::Faction Action::getTargetFaction()
{
	return myTargetFaction;
}