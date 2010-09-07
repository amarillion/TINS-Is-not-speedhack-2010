#include "Precompiled.hpp"
#include "ActionFactory.hpp"
#include "Action.hpp"
#include "ActionTrigger.hpp"

#include "ActionAttack.hpp"
#include "ActionElementalSpell.hpp"
#include "ActionHack.hpp"
#include "ActionIce2Spell.hpp"


Action * ActionFactory::createAction( const ActionTrigger &trigger, Character *performer, const std::vector<Character *>& targets )
{
	const std::string &name = trigger.getName();
	Action *result = NULL;

	if (name == "Attack") {
		result = new ActionAttack();
	} else if (name == "Fire 1") {
		result = new ActionElementalSpell("Fire 1", Character::Damage_Fire, 1);
	} else if (name == "Fire 2") {
		result = new ActionElementalSpell("Fire 2", Character::Damage_Fire, 2);
	} else if (name == "Fire 3") {
		result = new ActionElementalSpell("Fire 3", Character::Damage_Fire, 3);
	} else if (name == "Ice 1") {
		result = new ActionElementalSpell("Ice 1", Character::Damage_Ice, 1);
	} else if (name == "Ice 2") {
		result = new ActionIce2Spell();
	} else if (name == "Ice 3") {
		result = new ActionElementalSpell("Ice 3", Character::Damage_Ice, 3);
	} else if (name == "Bolt 1") {
		result = new ActionElementalSpell("Bolt 1", Character::Damage_Lighting, 1);
	} else if (name == "Bolt 2") {
		result = new ActionElementalSpell("Bolt 2", Character::Damage_Lighting, 2);
	} else if (name == "Bolt 3") {
		result = new ActionElementalSpell("Bolt 3", Character::Damage_Lighting, 3);
	} else if (name == "Cure 1") {
		result = new ActionElementalSpell("Cure 1", Character::Damage_Cure, 1);
	} else if (name == "Cure 2") {
		result = new ActionElementalSpell("Cure 2", Character::Damage_Cure, 2);
	} else if (name == "Cure 3") {
		result = new ActionElementalSpell("Cure 3", Character::Damage_Cure, 3);
	} else if(name == "Hack") {
		result = new ActionHack();
	}

	if (result == NULL)
	{
		throw std::string("Unable to create Action named " + name);
	}

	result->setActionTrigger(trigger);
	result->setPerformer(performer);
	result->setTargets(targets);

	return result;
}

Action * ActionFactory::createAction( const ActionTrigger &trigger, Character *performer, Character *target )
{
	std::vector<Character *> targets;
	targets.push_back(target);
	return createAction(trigger, performer, targets);
}

Action * ActionFactory::createAction( const ActionTrigger &trigger, Character *performer )
{
	return createAction(trigger, performer, std::vector<Character *>());
}