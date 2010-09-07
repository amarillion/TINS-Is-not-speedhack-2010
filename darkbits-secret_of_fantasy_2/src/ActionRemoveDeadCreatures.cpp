#include "Precompiled.hpp"
#include "ActionRemoveDeadCreatures.hpp"
#include "Battle.hpp"
#include "Character.hpp"

ActionRemoveDeadCreatures::ActionRemoveDeadCreatures() 
	: Action()
	, myDone(false)
{

}

ActionRemoveDeadCreatures::~ActionRemoveDeadCreatures()
{

}

void ActionRemoveDeadCreatures::onEnter( Battle *battle )
{
	std::vector<Character*> chars1 = battle->getFightingPlayerCharacters();
	std::vector<Character*> chars2 = battle->getEnemyCharacters();

	for (int i = 0; i < (int)chars1.size(); i++) {
		Character *c = chars1[i];
		if (c->getHP() <= 0) {
			c->enterDeadState();
			myDyingCharacters.push_back(c);
		}
	}

	for (int i = 0; i < (int)chars2.size(); i++) {
		Character *c = chars2[i];
		if (c->getHP() <= 0) {
			c->enterDeadState();
			myDyingCharacters.push_back(c);
		}
	}
}

void ActionRemoveDeadCreatures::onLeave()
{

}

void ActionRemoveDeadCreatures::onLogic()
{
	myDone = true;

	for (int i = 0; i < (int)myDyingCharacters.size(); i++) {
		if (!myDyingCharacters[i]->isDeadStateEntered()) {
			myDone = false;
		}
	}
}

bool ActionRemoveDeadCreatures::isDone()
{
	return myDone;
}