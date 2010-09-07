#pragma once

class Action;
class Character;
class ActionTrigger;

class ActionFactory {
public:
	static Action *createAction(const ActionTrigger &trigger, Character *performer, const std::vector<Character *>& targets);

	static Action *createAction(const ActionTrigger &trigger, Character *performer, Character *target);

	static Action *createAction(const ActionTrigger &trigger, Character *performer);
};