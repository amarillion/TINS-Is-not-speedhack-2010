#include "Precompiled.hpp"
#include "ActionTriggerLibrary.hpp"
#include "ActionTrigger.hpp"
#include "Action.hpp"
#include "ActionFactory.hpp"

class Character;

std::map<std::string, ActionTrigger> ActionTriggerLibrary::myTriggers;

void ActionTriggerLibrary::init()
{
	add(ActionTrigger(ActionTrigger::Type_Ability, "Attack", "Attack.", ActionTrigger::TargetMode_JustOne, ActionTrigger::TargetPolicy_OnlyAlive, ActionTrigger::Alignment_Offensive));
	add(ActionTrigger(ActionTrigger::Type_Ability, "Hack", "Sends a stream of computer code.", ActionTrigger::TargetMode_JustMany, ActionTrigger::TargetPolicy_OnlyAlive, ActionTrigger::Alignment_Offensive));

	//add(ActionTrigger(ActionTrigger::Type_Ability, "Defend", ActionTrigger::TargetMode_None, ActionTrigger::Alignment_Defensive));
	//add(ActionTrigger(ActionTrigger::Type_Ability, "Row", ActionTrigger::TargetMode_None, ActionTrigger::Alignment_Defensive));
	//add(ActionTrigger(ActionTrigger::Type_Ability, "Swap", ActionTrigger::TargetMode_NonFightingPartyMember, ActionTrigger::Alignment_Defensive));

	add(ActionTrigger(ActionTrigger::Type_Spell, "Cure 1", "Heal.", ActionTrigger::TargetMode_OneOrMany, ActionTrigger::TargetPolicy_OnlyAlive, ActionTrigger::Alignment_Defensive, 5));
	add(ActionTrigger(ActionTrigger::Type_Spell, "Cure 2", "Heal.",ActionTrigger::TargetMode_OneOrMany, ActionTrigger::TargetPolicy_OnlyAlive, ActionTrigger::Alignment_Defensive, 20));
	add(ActionTrigger(ActionTrigger::Type_Spell, "Cure 3", "Heal.",ActionTrigger::TargetMode_OneOrMany, ActionTrigger::TargetPolicy_OnlyAlive, ActionTrigger::Alignment_Defensive, 100));
	add(ActionTrigger(ActionTrigger::Type_Spell, "Fire 1", "Deal fire damage.",ActionTrigger::TargetMode_OneOrMany, ActionTrigger::TargetPolicy_OnlyAlive, ActionTrigger::Alignment_Offensive, 5));
	add(ActionTrigger(ActionTrigger::Type_Spell, "Fire 2", "Deal fire damage.", ActionTrigger::TargetMode_OneOrMany, ActionTrigger::TargetPolicy_OnlyAlive, ActionTrigger::Alignment_Offensive, 20));
	add(ActionTrigger(ActionTrigger::Type_Spell, "Fire 3",  "Deal fire damage.", ActionTrigger::TargetMode_OneOrMany, ActionTrigger::TargetPolicy_OnlyAlive, ActionTrigger::Alignment_Offensive, 100));
	add(ActionTrigger(ActionTrigger::Type_Spell, "Ice 1",  "Deal ice damage.", ActionTrigger::TargetMode_OneOrMany, ActionTrigger::TargetPolicy_OnlyAlive, ActionTrigger::Alignment_Offensive, 5));
	add(ActionTrigger(ActionTrigger::Type_Spell, "Ice 2", "Deal ice damage.", ActionTrigger::TargetMode_OneOrMany, ActionTrigger::TargetPolicy_OnlyAlive, ActionTrigger::Alignment_Offensive, 20));
	add(ActionTrigger(ActionTrigger::Type_Spell, "Ice 3", "Deal ice damage.", ActionTrigger::TargetMode_OneOrMany, ActionTrigger::TargetPolicy_OnlyAlive, ActionTrigger::Alignment_Offensive, 100));
	add(ActionTrigger(ActionTrigger::Type_Spell, "Bolt 1", "Deal lightning damage.", ActionTrigger::TargetMode_OneOrMany, ActionTrigger::TargetPolicy_OnlyAlive, ActionTrigger::Alignment_Offensive, 5));
	add(ActionTrigger(ActionTrigger::Type_Spell, "Bolt 2", "Deal lightning damage.", ActionTrigger::TargetMode_OneOrMany, ActionTrigger::TargetPolicy_OnlyAlive, ActionTrigger::Alignment_Offensive, 20));
	add(ActionTrigger(ActionTrigger::Type_Spell, "Bolt 3", "Deal lightning damage.", ActionTrigger::TargetMode_OneOrMany, ActionTrigger::TargetPolicy_OnlyAlive, ActionTrigger::Alignment_Offensive, 100));
}

void ActionTriggerLibrary::add( ActionTrigger &trigger )
{
	if (myTriggers.find(trigger.getName()) != myTriggers.end()) {
		throw std::string("Duplicate trigger registered for " + trigger.getName());
	}
	
	trigger.setOrder(myTriggers.size());
	myTriggers[trigger.getName()] = trigger;

	// Verify that the trigger maps to an action in ActionFactory
	delete ActionFactory::createAction(trigger, NULL);
}

const ActionTrigger & ActionTriggerLibrary::find( const std::string &name )
{
	if (myTriggers.find(name) == myTriggers.end()) {
		throw std::string("Unable to find trigger named " + name);
	}

	return myTriggers[name];
}
