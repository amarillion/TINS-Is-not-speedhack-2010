#pragma once

class ActionTrigger;

class ActionTriggerLibrary
{
public:
	static void init();

	static const ActionTrigger &find(const std::string &name);

private:
	ActionTriggerLibrary() {};
	
	static void add(ActionTrigger &trigger);

	static std::map<std::string, ActionTrigger> myTriggers;
};