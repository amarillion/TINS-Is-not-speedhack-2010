#pragma once

#include "ActionTrigger.hpp"
#include "Character.hpp"

class Character;
class Battle;

class Action {
public:
	Action();

	virtual ~Action();

	virtual void onEnter(Battle *battle) = 0;

	virtual void onLeave() = 0;

	virtual void onLogic() = 0;

	virtual bool isDone() = 0;

	void cancel();

	bool isCancelled();

	void setActionTrigger(const ActionTrigger &trigger);

	const ActionTrigger &getActionTrigger();

	const std::string &getName();

	Character *getPerformer();

	void setPerformer(Character *performer);

	int getNumTargets();

	Character *getTarget(int i);

	void setTargets(const std::vector<Character *> &targets);

	bool isTargetOfAction(Character *character);

	bool hadSingleTarget();

	Character::Faction getTargetFaction();

	void removeTarget(Character *target);

private:
	ActionTrigger myTrigger;
	bool myHadSingleTarget;
	bool myCancelled;
	Character *myPerformer;
	std::vector<Character *> myTargets;
	Character::Faction myTargetFaction;
};