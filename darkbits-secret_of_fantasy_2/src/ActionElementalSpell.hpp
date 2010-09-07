#pragma once

#include "Action.hpp"
#include "Character.hpp"
#include "Animation.hpp"
#include "ActionCast.hpp"

class ActionElementalSpell : public ActionCast {
public:
	ActionElementalSpell(const std::string& name, Character::Damage type, int spellLevel);

	void onPrepareCast();
	void onCastLogic();
	bool isCastDone();

private:
	std::string myName;
	Character::Damage myDamageType;
	int mySpellLevel;
	int myCastTime;
};
