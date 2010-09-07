#include "Precompiled.hpp"
#include "ActionIntroduceCharacter.hpp"
#include "Battle.hpp"
#include "Character.hpp"
  
ActionIntroduceCharacter::ActionIntroduceCharacter( Character *aCharacter )
: myIsDone(false)
{
	myCharacter = aCharacter;
}

void ActionIntroduceCharacter::onEnter( Battle *battle )
{
	if(!myIsDone) battle->addEntity(myCharacter);
	myIsDone = true;
	myBattle = battle;
}

void ActionIntroduceCharacter::onLeave()
{

}

void ActionIntroduceCharacter::onLogic()
{
	
}

bool ActionIntroduceCharacter::isDone()
{
	return myIsDone;
}