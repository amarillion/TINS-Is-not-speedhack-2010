#include "Precompiled.hpp"
#include "SetupBattle.hpp"
#include "Ted.hpp"
#include "Eve.hpp"
#include "EnemyCharacter.hpp"
#include "Fight.hpp"
#include "Robo.hpp"
#include "DialogueAction.hpp"
#include "PlayerCharacter.hpp"
#include "Mother.hpp"
#include "HackEntity.hpp"
#include "PortalEntity.hpp"
#include "ActionScript.hpp"
#include "ActionIntroduceCharacter.hpp"
#include "ActionTriggerLibrary.hpp"
#include "Gui.hpp"
#include "BattleMonitor.hpp"
#include "SkyBackground.hpp"
#include "ActionSleep.hpp"
#include "Music.hpp"
#include "SpaceBackground.hpp"

Ted* ted;
Mother* mother;
Robo* robo;

//////////////////////////////////////////////////////////////////////////
// GENERALS
//////////////////////////////////////////////////////////////////////////
void createPortalEntity(Battle *battle) {
	battle->addEntity(new PortalEntity()); 
};

void inactivateTed(Battle *battle) {
	Character *ted = battle->findCharacter("TED");
	PlayerCharacter *playerCharacter = dynamic_cast<PlayerCharacter*>(ted);
	playerCharacter->setFighting(false);
	
}

void activateTed(Battle *battle) {
	Character *ted = battle->findCharacter("TED");
	PlayerCharacter *playerCharacter = dynamic_cast<PlayerCharacter*>(ted);
	playerCharacter->setFighting(true);
}

void disableGui(Battle* battle)
{
	battle->getGui()->setInteractable(false);
}

void enableGui(Battle* battle)
{
	battle->getGui()->setInteractable(true);
}

//////////////////////////////////////////////////////////////////////////
// INTRO
//////////////////////////////////////////////////////////////////////////
void scriptIntroSetup(Battle* battle)
{
	ted->setHome(280, 110);
	ted->setPosition(280, 110);
	mother->setHome(280, 150);
	mother->setPosition(280, 150);
}
void scriptIntroHideTed(Battle* battle)
{
	Character* ted = battle->findCharacter("TED");
	ted->enterHideState();
}

void scriptIntroShowTed(Battle* battle)
{
	Character* ted = battle->findCharacter("TED");
	ted->enterIdleState();
}

void scriptIntroGodFlee(Battle* battle)
{
	Character *god = battle->findCharacter("?");
	battle->getGui()->showMessage("? flees.");
	god->flee();
}

void scriptIntroMusicChange(Battle* battle)
{
	Music::playSong("data/music/olof7.xm");
}

bool monitorIntro(Battle* battle)
{
	Character *mother = battle->findCharacter("MOTHER");
	if(mother != 0 && mother->isDead()) 
	{
		battle->enqueueAction(new ActionSleep(60));
		DialogueAction* da = new DialogueAction();
		da->addText("?: Hahaha! It is done! It is finally done!");
		da->addText("?: ....");
		da->addText("?: But I am still here? HOW CAN THIS BE!!!");
		battle->enqueueAction(da);
		battle->enqueueAction(new ActionSleep(60));
		battle->enqueueAction(new ActionScript(scriptIntroShowTed));
		battle->enqueueAction(new ActionSleep(120));
		da = new DialogueAction();
		da->addText("TED: NOOO! Mother!!!?");
		da->addText("?: I was to late...");
		battle->enqueueAction(da);
		battle->enqueueAction(new ActionSleep(60));
		battle->enqueueAction(new ActionScript(createPortalEntity));
		battle->enqueueAction(new ActionSleep(60));
		battle->enqueueAction(new ActionScript(scriptIntroGodFlee));
		battle->enqueueAction(new ActionSleep(60));
		da = new DialogueAction();
		da->addText("TED: He disappeared in a time portal!");
		da->addText("TED: Mother I will avenge you!");
		battle->enqueueAction(da);

		return true;
	}

	return false;
}

Fight *createFight1() 
{
	Fight* fight = new Fight();
	fight->setMusic("data/music/menu.xm");

	EnemyCharacter* god;
	god = new EnemyCharacter("?", "god.bmp", 100, 140, 100, 10, 10, 0.5f);
	god->addActionTrigger("Attack");

	fight->addIntroAction(new ActionScript(scriptIntroSetup));
	fight->addIntroAction(new ActionScript(inactivateTed));
	fight->addBattleMonitor(new BattleMonitor(monitorIntro));
	fight->addEntity(new SkyBackground());
	fight->addIntroAction(new ActionScript(disableGui));
	DialogueAction* da = new DialogueAction();
	da->addText("TED: It's such a beautiful day mother.\n"
				"I wish it would last forever...");
	da->addText("MOTHER: Ah yes my son. It sure is.");
	da->addText("MOTHER: Now we really should get back to Paradise town. We don't want to miss your birthday party.");
	da->addText("TED: That's right mother.");
	da->addText("TED: Before we go there is a small thing I want to do.");
	da->addText("TED: I think I saw some wonderful flowers at the other hill, I want to go and get them for you.");
	da->addText("MOTHER: You are so sweet. Hurry dear!");
	fight->addIntroAction(da);

	fight->addIntroAction(new ActionScript(scriptIntroHideTed));
	fight->addIntroAction(new ActionSleep(120));
	fight->addIntroAction(new ActionIntroduceCharacter(god));
	fight->addIntroAction(new ActionSleep(120));
	da = new DialogueAction();
	da->addText("?: Finally I found you.");
	da->addText("?: Now lets end this!");
	fight->addIntroAction(da);
	fight->addIntroAction(new ActionScript(scriptIntroMusicChange));
	da = new DialogueAction;
	da->addText("MOTHER: Help!!!");
	fight->addIntroAction(da);
	fight->addOutroAction(new ActionScript(enableGui));
	fight->addOutroAction(new ActionScript(createPortalEntity));
	fight->addOutroAction(new ActionScript(activateTed));


//	fight->addEntity(new HackEntity(0, 0));
// 	e = new EnemyCharacter( "ENEMY1", "sandmystic.bmp", 40, 130, 10, 10, 10, 10, 0.1f);
// 	e->addActionTrigger("Attack");
// 	fight->addEntity(e);
// 
// 	e = new EnemyCharacter( "ENEMY1", "angryflower.bmp", 100, 80, 10, 10, 10, 10, 0.1f);
// 	e->addActionTrigger("Attack");
// 	fight->addEntity(e);
// 
// 	e = new EnemyCharacter( "ENEMY1", "hugegod.bmp", 110, 140, 10, 10, 10, 10, 0.1f);
// 	e->addActionTrigger("Attack");
// 	fight->addEntity(e);



	return fight;
}

//////////////////////////////////////////////////////////////////////////
// FIGHT 2
//////////////////////////////////////////////////////////////////////////
void script2ShowRobo(Battle* battle)
{
	robo->enterIdleState();
}

void script2Setup(Battle* battle)
{
	ted->setHome(280, 110);
	ted->setPosition(280, 110);
	ted->setFighting(true);
	mother->setFighting(false);
	mother->setPosition(640, 0);
	mother->setHome(640, 0);
	robo->setHome(280, 150);
	robo->setPosition(350, 150);

	ted->addActionTrigger("Cure 1");
	ted->addActionTrigger("Ice 2");
	robo->addActionTrigger("Hack");

	battle->addEntity(robo);
	mother->remove();

	battle->getGui()->setInteractable(false);
	
	battle->enqueueAction(new ActionSleep(60));
	DialogueAction* da;
	da = new DialogueAction();
	da->addText("TED: Where did he go?");
	battle->enqueueAction(da);
	battle->enqueueAction(new ActionSleep(60));	
	
	EnemyCharacter* e;
	e = new EnemyCharacter("SPACE WHALE 1", "whale.bmp", 100, 140, 2, 600, 10, 0.2f);
	e->addActionTrigger("Attack");
	battle->enqueueAction(new ActionIntroduceCharacter(e));
	e = new EnemyCharacter("SPACE WHALE 2", "whale.bmp", 80, 100, 3, 700, 10, 0.1f);
	e->addActionTrigger("Attack");
	battle->enqueueAction(new ActionIntroduceCharacter(e));
	
	battle->enqueueAction(new ActionSleep(60));	
	da = new DialogueAction();
	da->addText("TED: Whales?");
	da->addText("ROBO: THOSE ARE SPACE WHALES. I WILL HELP. END OF MESSAGE.");
	battle->enqueueAction(da);
	battle->enqueueAction(new ActionSleep(60));	
	
	battle->enqueueAction(new ActionScript(script2ShowRobo));
	battle->enqueueAction(new ActionSleep(60));
	battle->enqueueAction(new ActionScript(enableGui));
}

Fight *createFight2() {

	Fight* fight = new Fight();
	fight->setMusic("data/music/olof6.xm");
	fight->addEntity(new SpaceBackground());
	fight->addIntroAction(new ActionScript(script2Setup));
	
	DialogueAction* da = new DialogueAction();
	da->addText("TEAM DARKBITS: Sorry guys, but there isn't much more game play at the moment.");
	da->addText("TEAM DARKBITS: One of us got sick during the compo and we just didn't plan for that.");
	da->addText("TEAM DARKBITS: Anyway, enough with the whining. Consider this a preview of our game.");
	da->addText("TEAM DARKBITS: Hopefully we'll complete a more fun and playable version in the future.");
	da->addText("TEAM DARKBITS: Thanks for playing this far! And thank you Amarillion for TINS!");
	da->addText("TEAM DARKBITS: Over and out from Team Darkbits.");
	fight->addOutroAction(da);
	return fight;
}

extern Battle* SetupBattle()
{
	Battle *battle = new Battle();

	ted = new Ted(0, 0);
	mother = new Mother(0, 0);
	robo = new Robo(0, 0);
	battle->addEntity(ted);
	battle->addEntity(mother);

	battle->enqueueFight(createFight1());
	battle->enqueueFight(createFight2());
	return battle;
}
