#pragma once

#include "Action.hpp"
#include "Dialogue.hpp"

class DialogueAction: public Action
{
public:

	 void onEnter(Battle *battle);
	 void onLeave();
	 void onLogic();
	 bool isDone();
	 void addText(const std::string& text);

private:
	Battle* myBattle;
	Dialogue myDialogue;
};