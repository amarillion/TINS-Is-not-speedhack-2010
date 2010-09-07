#include "Precompiled.hpp"
#include "DialogueAction.hpp"
#include "Battle.hpp"
#include "Gui.hpp"

void DialogueAction::onEnter( Battle *battle )
{
	myBattle = battle;
	myBattle->getGui()->showDialogue(myDialogue);
}

void DialogueAction::onLeave()
{

}

void DialogueAction::onLogic()
{

}

bool DialogueAction::isDone()
{
	return myBattle->getGui()->isDialogueDone();
}

void DialogueAction::addText( const std::string& text )
{
	myDialogue.addText(text);
}
