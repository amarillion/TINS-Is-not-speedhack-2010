#include "Precompiled.hpp"
#include "Gui.hpp"
#include "Resource.hpp"
#include "ScreenManager.hpp"
#include "GuiDialogue.hpp"
#include "Util.hpp"
#include "Character.hpp"
#include "PlayerCharacter.hpp"
#include "Font.hpp"
#include "Battle.hpp"
#include "Input.hpp"
#include "Constants.hpp"
#include "EnemyCharacter.hpp"
#include "ActionTrigger.hpp"
#include "ActionTriggerLibrary.hpp"
#include "ActionFactory.hpp"
#include "Sound.hpp"

Gui::Gui(Battle* aBattle)
:Screen(true)
, myBattle(aBattle)
,myMessageBackground("data/images/gui_message_background.bmp", 1)
,myInfoMessageBackground("data/images/gui_info_message_background.bmp", 1)
,myBackground("data/images/gui_background.bmp", 1)
,myShowTitle(false)
,myCharacterTimer("data/images/gui_character_timer.bmp", 1)
,myDialogueDone(true)
,myGuiDialogue(0)
,myActionBackground("data/images/gui_action_background.bmp", 1)
,myHand("data/images/gui_hand.bmp", 1)
,myMarker("data/images/gui_marker.bmp", 1)
,myArrowRight("data/images/gui_arrow_right.bmp", 1)
,myArrowLeft("data/images/gui_arrow_left.bmp", 1)
,mySelectTargetState(SelectTarget_None)
,mySelectAction(false)
,mySelectActionDone(false)
,myInteractable(false)
{
	myFont = Resource::getFont("data/images/font.bmp");	
	myFontDisabled = Resource::getFont("data/images/font.bmp", makecol(80, 80, 80));
	myHighlightedFont = Resource::getFont("data/images/font.bmp", makecol(255, 255, 0));
}

void Gui::onDraw( BITMAP* buffer )
{
	myBackground.drawFrame(buffer, 0, 0, 161);
	
	myInfoMessageBackground.drawFrame(buffer, 0, 0, 220);
	myFont->drawCenterY(buffer, myInfoMessageLeft, 10, 220, myInfoMessageBackground.getFrameHeight());
	myFont->drawCenterYAlignRight(buffer, myInfoMessageRight, 310, 220, myInfoMessageBackground.getFrameHeight());

	privDrawTitle(buffer);

	if (!myInteractable)
		return;

	privDrawPlayerInfo(buffer);
	privDrawEnemyInfo(buffer);

	if (myBattle->isAllEnemiesDead())
	{
		return;
	}

	privDrawSelectAction(buffer);
	privDrawSelectTarget(buffer);
}

void Gui::onLogic()
{
	myCounter++;

	myInfoMessageLeft = "";
	myInfoMessageRight = "";

	privLogicTitle();
	privLogicDialogue();

	if (!myInteractable)
		return;

	privLogicPlayerInfo();

	if (myBattle->isAllEnemiesDead())
	{
		return;
	}

	privLogicSelectAction();
	privLogicSelectTarget();
}

void Gui::showMessage( const std::string& aTitle )
{
	myTitle = aTitle;
	myTitleCounter = 60 * 2;
	myShowTitle = true;
}

void Gui::showDialogue( const Dialogue& aDialogue )
{
	myGuiDialogue = new GuiDialogue(aDialogue);
	ScreenManager::add(myGuiDialogue);
	myDialogueDone = false;
}

void Gui::privLogicDialogue()
{
	if (ScreenManager::getTop() == myGuiDialogue)
	{
		if (myGuiDialogue->isDone())
		{
			myDialogueDone = true;
			ScreenManager::exit(myGuiDialogue);

		}
	}
}

void Gui::privLogicTitle()
{
	if (myShowTitle)
	{
		myTitleCounter--;
		if (myTitleCounter < 0)
		{
			myShowTitle = false;
		}
	}
}

void Gui::privDrawTitle( BITMAP* buffer )
{
	if (myShowTitle)
	{
		myMessageBackground.drawFrame(buffer, 0, 0, 5);
		myFont->drawCenter(buffer, myTitle, 0, 5, 320, 20);
	}
}

void Gui::privLogicPlayerInfo()
{
	
}

void Gui::privDrawPlayerInfo( BITMAP* buffer )
{
	std::vector<PlayerCharacter*> players = myBattle->getAllPlayerCharacters();

	for (unsigned int i = 0; i < players.size(); i++)
	{
		PlayerCharacter* p =  players[i];
		privDrawPlayerInfo(buffer,p, i);
	}
}

void Gui::privDrawPlayerInfo( BITMAP* buffer, Character* aCharacter, int aYOrder )
{
	Font* font = myFont;

	if (myPlayerCharacterWithAction == aCharacter)
	{
		font = myHighlightedFont;
	}
	if (aCharacter->isDead())
	{
		font = myFontDisabled;
	}

	int y = 164 + aYOrder * (myFont->getHeight() + 1) + 3;
	font->draw(buffer, aCharacter->getName(), 133, y);
	font->draw(buffer, intToStringFixed4(aCharacter->getHP()), 190, y);
	font->draw(buffer, intToStringFixed3(aCharacter->getMP()), 230, y);
	myCharacterTimer.drawFrame(buffer, 0, 258, y + 1);
	int w = 50 * aCharacter->getTime();

	int color;

	if (w == 50)
	{
		color = makecol(253, 210, 78);
	}
	else
	{
		color = makecol(255, 255, 255);
	}
	line(buffer, 260, y + 4, 260 + w, y + 4, color);
}

void Gui::privResetSelectAction()
{
	mySelectActionFocus = SelectActionFocus_None;
	mySelectActionDone = false;
	mySelectAction = false;
}

void Gui::privSwitchToSelectAction(bool aRemberLastPosition)
{
	mySelectActionDone = false;
	if (!aRemberLastPosition)
	{
		privSwitchToSelectActionMain(true);
	}
}

void Gui::privLogicSelectAction()
{
	if (myPlayerCharacterWithAction != myBattle->getPlayerCharacterWithActionChoice())
	{
		myPlayerCharacterWithAction = myBattle->getPlayerCharacterWithActionChoice();

		if (myPlayerCharacterWithAction != 0)
		{
			mySelectAction = true;
			privSwitchToSelectActionMain(false);
			mySelectActionDone = false;
		}
		else
		{
			mySelectAction = false;
			mySelectActionDone = false;
		}
	}

	if (ScreenManager::getTop() != this)
	{
		return;
	}

	if (!mySelectAction)
	{
		return;
	}

	if (mySelectActionFocus == SelectActionFocus_Main)
	{
		privLogicSelectActionMain();
	}
	else if (mySelectActionFocus == SelectActionFocus_Magic)
	{
		privLogicSelectActionMagic();
	}
	else if (mySelectActionFocus == SelectActionFocus_Secondary)
	{
		privLogicSelectActionSecondary();
	}
	else if (mySelectActionFocus == SelectActionFocus_Item)
	{
		privLogicSelectActionItem();
	}
}

void Gui::privDrawSelectAction( BITMAP* buffer )
{
	if (!mySelectAction)
	{
		return;
	}

	if (ScreenManager::getTop() != this)
	{
		return;
	}

	if (mySelectActionFocus == SelectActionFocus_Main)
	{
		privDrawSelectActionMain(buffer);
	}
	else if (mySelectActionFocus == SelectActionFocus_Magic)
	{
		privDrawSelectActionMagic(buffer);
	}
	else if (mySelectActionFocus == SelectActionFocus_Secondary)
	{
		privDrawSelectActionSecondary(buffer);
	}
	else if (mySelectActionFocus == SelectActionFocus_Item)
	{
		privDrawSelectActionItem(buffer);
	}
}

void Gui::privSwitchToSelectActionMain(bool aRememberLast)
{
	Sound::playSample("data/sound/gui_select.wav");
	mySelectActionFocus = SelectActionFocus_Main;
	mySelectMainFrameCounter = 0;
	
	if (!aRememberLast)
	{
		mySelectActionMainRow = 0;
	}
}

void Gui::privLogicSelectActionMain()
{
	mySelectMainFrameCounter++;

	if (mySelectActionDone)
	{
		return;
	}

	if (mySelectActionFocus != SelectActionFocus_Main)
	{
		return;
	}

	if (mySelectActionMainRow == 0)
	{
		myInfoMessageLeft = "Attack an enemy.";
	}
	else if (mySelectActionMainRow == 1)
	{
		myInfoMessageLeft = "Use magic.";
	}
	else if (mySelectActionMainRow == 2)
	{
		ActionTrigger trigger = *myPlayerCharacterWithAction->getSpecialActionTriggers().begin();
		myInfoMessageLeft = trigger.getDescription();
	}
	else if (mySelectActionMainRow == 3)
	{
		myInfoMessageLeft = "Use item.";
	}

	if (ScreenManager::getTop() != this)
		return;

	if (Input::isPressed(Button_Exit))
	{
		myBattle->placePlayerWithActionChoiceLastInQueue();
		return;
	}

// 	if (Input::isPressed(Button_Right))
// 	{
// 		privSwitchToSelectActionSecondary();
// 		return;
// 	}

	if (Input::isPressed(Button_Up))
	{
		mySelectActionMainRow--;
		
		if (mySelectActionMainRow == 2 && myPlayerCharacterWithAction->getSpecialActionTriggers().size() == 0)
		{
			mySelectActionMainRow--;
		}
		if (mySelectActionMainRow == 1 && myPlayerCharacterWithAction->getSpellActionTriggers().size() == 0)
		{
			mySelectActionMainRow--;
		}

		if (mySelectActionMainRow < 0)
		{
			mySelectActionMainRow = 0;
		}
		else
		{
			Sound::playSample("data/sound/gui_select.wav");
		}
	}



	if (Input::isPressed(Button_Down))
	{
		mySelectActionMainRow++;

		if (mySelectActionMainRow > 2)
		{
			mySelectActionMainRow = 2;
		}
		else
		{
			Sound::playSample("data/sound/gui_select.wav");
		}

		if (myPlayerCharacterWithAction->getSpellActionTriggers().size() == 0
			&& myPlayerCharacterWithAction->getSpecialActionTriggers().size() == 0)
		{
			mySelectActionMainRow = 0;
		}
		else
		{
			if (mySelectActionMainRow == 1 && myPlayerCharacterWithAction->getSpellActionTriggers().size() == 0)
			{
				mySelectActionMainRow++;
			}
			if (mySelectActionMainRow == 2 && myPlayerCharacterWithAction->getSpecialActionTriggers().size() == 0)
			{
				mySelectActionMainRow--;
			}
		}
	}

	if (Input::isPressed(Button_Action))
	{		
		if (mySelectActionMainRow == 0)
		{
			privDoSelectActionAttack();
		}
		else if (mySelectActionMainRow == 1)
		{
			privSwitchToSelectActionMagic();
		}
		else if (mySelectActionMainRow == 2)
		{
			privDoSelectActionSpecial();
		}
// 		else if (mySelectActionMainRow == 3)
// 		{
// 			privSwitchToSelectActionItem();
// 		}
	}
}

void Gui::privDrawSelectActionMain( BITMAP* buffer )
{
	myActionBackground.drawFrame(buffer, 0, 0, 161);

	int x = 37;
	int y = 167;
	myFont->draw(buffer, "Attack", x, y);
	if (myPlayerCharacterWithAction->getSpellActionTriggers().size() != 0)
	{
		myFont->draw(buffer, "Magic", x, y+ myFont->getHeight()+1);
	}
	if (myPlayerCharacterWithAction->getSpecialActionTriggers().size() != 0)
	{
		ActionTrigger trigger = *myPlayerCharacterWithAction->getSpecialActionTriggers().begin();
		myFont->draw(buffer, trigger.getName(), x, y + myFont->getHeight()*2+1);
	}
	
	//myFont->draw(buffer, "Item", x, y + myFont->getHeight()*3 + 1);

	if (!mySelectActionDone || mySelectMainFrameCounter % 8 < 4)
	{
		myHand.drawFrame(buffer, 0, x - 13, y + myFont->getHeight() * mySelectActionMainRow);
	}

// 	if (mySelectActionFocus == SelectActionFocus_Main)
// 	{
// 		myArrowRight.drawFrame(buffer, 0, x + 70, y + 18);
// 	}
}

void Gui::privSwitchToSelectActionSecondary()
{
	Sound::playSample("data/sound/gui_select.wav");
	mySelectActionFocus = SelectActionFocus_Secondary;
	mySelectSecondaryFrameCounter = 0;
	mySelectActionSecondaryRow = 0;
}

void Gui::privLogicSelectActionSecondary()
{
	mySelectSecondaryFrameCounter++;

	if (mySelectActionDone)
	{
		return;
	}

	if (mySelectActionFocus != SelectActionFocus_Secondary)
	{
		return;
	}

// 	if (Input::isPressed(Button_Left))
// 	{		
// 		mySelectActionState = SelectAction_Main;
// 		mySelectActionRow = 0;	
// 	}
// 
	if (Input::isPressed(Button_Up))
	{
		mySelectActionSecondaryRow--;

		if (mySelectActionSecondaryRow < 0)
		{
			mySelectActionSecondaryRow = 0;
		}
		else
		{
			Sound::playSample("data/sound/gui_select.wav");
		}
	}

	if (Input::isPressed(Button_Down))
	{
		mySelectActionSecondaryRow++;

		if (mySelectActionSecondaryRow > 2)
		{
			mySelectActionSecondaryRow = 2;
		}
		else
		{
			Sound::playSample("data/sound/gui_select.wav");
		}
	}

	if (Input::isPressed(Button_Left))
	{
		privSwitchToSelectActionMain(false);
	}

	if (mySelectActionSecondaryRow == 0)
	{
		myInfoMessageLeft = "Defend from attacks.";
	}
	else if (mySelectActionSecondaryRow == 1)
	{
		myInfoMessageLeft = "Swap in another player.";
	}
	else if (mySelectActionSecondaryRow == 2)
	{
		myInfoMessageLeft = "Equip an item.";
	}
}

void Gui::privDrawSelectActionSecondary( BITMAP* buffer )
{
 	myActionBackground.drawFrame(buffer, 0, 0, 161);

	int x = 37;
 	int y = 167;
	myFont->draw(buffer, "Defend", x, y);
	myFont->draw(buffer, "Swap", x, y+ myFont->getHeight());
	myFont->draw(buffer, "Equip", x, y+ myFont->getHeight()*2);
 
 	if (!mySelectActionDone || mySelectSecondaryFrameCounter % 8 < 4)
 	{
 		myHand.drawFrame(buffer, 0, x - 13, y + myFont->getHeight() * mySelectActionSecondaryRow);
 	}

	if (mySelectActionFocus == SelectActionFocus_Secondary)
	{
		myArrowLeft.drawFrame(buffer, 0, x - 27, y + 18);
	}
}

void Gui::privSwitchToSelectActionMagic()
{
	Sound::playSample("data/sound/gui_select.wav");
	mySelectActionFocus = SelectActionFocus_Magic;
	mySelectActionMagicNextFrame = true;
	mySelectActionMagicScroll = 0;
	mySelectActionMagicHandRow = 0;
	mySelectActionMagicFrameCounter = 0;
	mySelectActionMagicRow = 0;
}

void Gui::privLogicSelectActionMagic()
{
	if (mySelectActionMagicNextFrame)
	{
		mySelectActionMagicNextFrame = false;
		return;
	}

	mySelectActionMagicFrameCounter++;

	if (mySelectActionDone)
	{
		return;
	}

	if (mySelectActionFocus != SelectActionFocus_Magic)
	{
		return;
	}

	if (Input::isPressed(Button_Exit))
 	{
		privSwitchToSelectActionMain(true);
		return;
	}

	if (Input::isPressed(Button_Action))
	{
		privDoSelectActionMagic(mySelectActionMagicRow);
	}

	if (Input::isPressed(Button_Up))
	{
		mySelectActionMagicRow--;
		mySelectActionMagicHandRow--;
		if (mySelectActionMagicHandRow < 0)
		{
			mySelectActionMagicScroll--;
		}

		if (mySelectActionMagicRow < 0)
		{
			mySelectActionMagicRow = 0;
		}
		else
		{
			Sound::playSample("data/sound/gui_select.wav");
		}
		
		mySelectActionMagicScroll = clamp(mySelectActionMagicScroll, 0, mySelectActionMagicScroll);
		mySelectActionMagicHandRow = clamp(mySelectActionMagicHandRow, 0, mySelectActionMagicHandRow);
	}

	if (Input::isPressed(Button_Down))
	{
		int numberOfSpells = myPlayerCharacterWithAction->getSpellActionTriggers().size();
		
		if (numberOfSpells < 5)
		{
			mySelectActionMagicScroll = 0;
			mySelectActionMagicHandRow++;
			mySelectActionMagicRow++;

			if (mySelectActionMagicRow > numberOfSpells - 1)
			{
				mySelectActionMagicRow = numberOfSpells -1;
				mySelectActionMagicHandRow = numberOfSpells -1;
			}
			else
			{
				Sound::playSample("data/sound/gui_select.wav");
			}
		}
		else
		{		
			mySelectActionMagicHandRow++;
			mySelectActionMagicRow++;
			if (mySelectActionMagicHandRow > 3)
			{
				mySelectActionMagicScroll++;
			}

			if (mySelectActionMagicScroll > numberOfSpells - 4)
			{
				mySelectActionMagicScroll = numberOfSpells - 4;
				if (numberOfSpells < 4)
				{
					mySelectActionMagicHandRow = numberOfSpells - 1;
				}
				else
				{
					mySelectActionMagicHandRow = 3;
				}
			}

			if (mySelectActionMagicRow > numberOfSpells - 1)
			{
				mySelectActionMagicRow = numberOfSpells -1;
			}
			else
			{
				Sound::playSample("data/sound/gui_select.wav");
			}

			mySelectActionMagicHandRow = clamp(mySelectActionMagicHandRow, 0, 3);
		}
	}

	int i = 0;
	std::set<ActionTrigger> spells = myPlayerCharacterWithAction->getSpellActionTriggers();
	for (std::set<ActionTrigger>::iterator it = spells.begin(); it != spells.end(); it++)
	{
		if (i == mySelectActionMagicRow)
		{
			ActionTrigger trigger = (*it);
			
			myInfoMessageLeft = trigger.getDescription();
			myInfoMessageRight = "MP:" + toString(trigger.getMPCost());

			break;
		}
		i++;
	}

}

void Gui::privDrawSelectActionMagic( BITMAP* buffer )
{
 	myActionBackground.drawFrame(buffer, 0, 0, 161);
 	int x = 37;
	int y = 167;
 	int i = 0;
 	std::set<ActionTrigger> spells = myPlayerCharacterWithAction->getSpellActionTriggers();

	int dy = y;
 	for (std::set<ActionTrigger>::iterator it = spells.begin(); it != spells.end(); it++)
 	{	
		if (i >= mySelectActionMagicScroll && i < mySelectActionMagicScroll + 4)
		{
			ActionTrigger trigger = (*it);
			Font* font = myFont;
			if (trigger.getMPCost() > myPlayerCharacterWithAction->getMP())
			{
				font = myFontDisabled;
			}
			font->draw(buffer, trigger.getName(), x, dy);
			dy += myFont->getHeight();
		}
 	
 		i++;
 	}
 
	if (!mySelectActionDone || mySelectActionMagicFrameCounter % 8 < 4)
	{
		myHand.drawFrame(buffer, 0, x - 13, y + myFont->getHeight() * mySelectActionMagicHandRow);
	}
}

void Gui::privSwitchToSelectActionItem()
{
	mySelectActionFocus = SelectActionFocus_Item;
}

void Gui::privLogicSelectActionItem()
{
	if (mySelectActionDone)
	{
		return;
	}

	if (mySelectActionFocus != SelectActionFocus_Item)
	{
		return;
	}
}

void Gui::privDrawSelectActionItem( BITMAP* buffer )
{

}

void Gui::privSwitchToSelectTarget()
{
	mySelectTargetNextFrame = true;
	if (mySelectedAction.getAlignment() == ActionTrigger::Alignment_Defensive)
	{
		mySelectTargetState = SelectTarget_Players;
	}
	else
	{
		mySelectTargetState = SelectTarget_Enemies;
	}

	mySelectTargetAll = false;
	mySelectTargetFrameCounter = 0;
	mySelectedTargets.clear();
	Sound::playSample("data/sound/gui_select.wav");
}

void Gui::privDrawSelectTarget( BITMAP* buffer )
{
	if (ScreenManager::getTop() != this)
	{
		return;
	}

	if (mySelectTargetState == SelectTarget_None)
		return;

	if (mySelectedTargets.size() == 0)
		return;

	if (mySelectedTargets.size() == 1)
	{
		Character* c = mySelectedTargets[0];
		int x = c->getMarkerX() - myMarker.getFrameWidth() / 2;
		int y = c->getMarkerY() - myMarker.getFrameHeight() / 2;
		myMarker.drawFrame(buffer, 0, x, y);
		return;
	}
	{
		int i = (mySelectTargetFrameCounter / 4) % (mySelectedTargets.size());
		Character* c = mySelectedTargets[i];
		int x = c->getMarkerX() - myMarker.getFrameWidth() / 2;
		int y = c->getMarkerY() - myMarker.getFrameHeight() / 2;
		myMarker.drawFrame(buffer, 0, x, y);
	}
}

void Gui::privLogicSelectTarget()
{
	mySelectTargetFrameCounter++;

	if (mySelectTargetState == SelectTarget_None)
	{
		return;
	}

	if (mySelectTargetNextFrame)
	{
		mySelectTargetNextFrame = false;
		return;
	}

	if (Input::isPressed(Button_Exit))
	{
		mySelectTargetState = SelectTarget_None;
		privSwitchToSelectAction(true);
		return;
	}

	privLogicSelectTargetCheckSelected();
	privLogicSelectTargetAll();
	privLogicSelectTargetCheckSelected();
	privLogicSelectTargetMakeSureSomethingIsSelected();
	privLogicSelectTargetSingle();

	if (mySelectedTargets.size() == 0)
	{
		privSwitchToSelectAction(true);
		return;
	}

	privLogicSelectTargetAction();	
}

void Gui::privLogicSelectTargetCheckSelected()
{
	std::vector<Character*> targets;
	for (unsigned int i = 0; i < mySelectedTargets.size(); i++)
	{
		Character* c = mySelectedTargets[i];
		if (!myBattle->isOnBattlefield(c))
			continue;

		if (mySelectedAction.getTargetPolicy() == ActionTrigger::TargetPolicy_OnlyAlive
			&& c->getHP() == 0)
			continue;

		targets.push_back(c);
	}

	mySelectedTargets.clear();
	mySelectedTargets = targets;
}

void Gui::privLogicSelectTargetAll()
{
	if (ScreenManager::getTop() != this)
	{
		return;
	}

	if (mySelectTargetAll 
		&& Input::isPressed(Button_All)
		&& mySelectedAction.getTargetMode() != ActionTrigger::TargetMode_JustMany)
	{
		mySelectTargetAll = false;
		mySelectedTargets.clear();
		Sound::playSample("data/sound/gui_select.wav");
		return;
	}

	if (mySelectedAction.getTargetMode() != ActionTrigger::TargetMode_JustMany
		&& !(Input::isPressed(Button_All) && mySelectedAction.getTargetMode() == ActionTrigger::TargetMode_OneOrMany))
		return;

	if (mySelectTargetAll)
		return;

	if (mySelectTargetState == SelectTarget_Enemies)
	{
		mySelectedTargets = myBattle->getEnemyCharacters();
	}
	else
	{
		mySelectedTargets = myBattle->getFightingPlayerCharacters();
	}

	mySelectTargetAll = true;
	Sound::playSample("data/sound/gui_select.wav");
}

void Gui::privLogicSelectTargetMakeSureSomethingIsSelected()
{
	if (mySelectedTargets.size() != 0)
		return;


	if (mySelectTargetState == SelectTarget_Players)
	{
		if (!(mySelectedAction.getTargetPolicy() == ActionTrigger::TargetPolicy_OnlyAlive
			&& myPlayerCharacterWithAction->getHP() == 0))
		{
			mySelectedTargets.push_back(myPlayerCharacterWithAction);
		}
	}
	else
	{
		std::vector<Character*> characters = myBattle->getEnemyCharacters();
		for (unsigned int i = 0; i < characters.size(); i++)
		{
			Character* c = characters[i];
			if (mySelectedAction.getTargetPolicy() == ActionTrigger::TargetPolicy_OnlyAlive 
				&& c->getHP() == 0)
				continue;
			mySelectedTargets.push_back(c);
			return;
		}
	}
}

void Gui::privLogicSelectTargetSingle()
{
	if (ScreenManager::getTop() != this)
	{
		return;
	}

	if (mySelectedTargets.size() != 1)
		return;
	
	Character* currentSelected = mySelectedTargets[0];
	Character* oldSelected = currentSelected;

	mySelectedTargets.clear();

	if (Input::isPressed(Button_Up))
	{
		int distance = 100000000;
		Character* candidate = 0;
		std::vector<Character*> characters;
		if (mySelectTargetState == SelectTarget_Players)
			characters = myBattle->getFightingPlayerCharacters();
		else
			characters = myBattle->getEnemyCharacters();

		for (unsigned int i = 0; i< characters.size(); i++)
		{
			Character* c = characters[i];
		
			if (currentSelected->getMarkerY() <= c->getMarkerY())
				continue;

			if (mySelectedAction.getTargetPolicy() == ActionTrigger::TargetPolicy_OnlyAlive
				&& c->getHP() == 0)
				continue;
			
			int d = privGetDistanceSquare(c, currentSelected, 1, 2);
			if (d < distance)
			{
				candidate = c;
				distance = d;
			}
		}	

		if (candidate != 0)
			currentSelected = candidate;
	}

	if (Input::isPressed(Button_Down))
	{
		int distance = 100000000;
		Character* candidate = 0;
		std::vector<Character*> characters;
		if (mySelectTargetState == SelectTarget_Players)
			characters = myBattle->getFightingPlayerCharacters();
		else
			characters = myBattle->getEnemyCharacters();

		for (unsigned int i = 0; i < characters.size(); i++)
		{
			Character* c = characters[i];
			
			if (currentSelected->getMarkerY() >= c->getMarkerY())
				continue;

			if (mySelectedAction.getTargetPolicy() == ActionTrigger::TargetPolicy_OnlyAlive
				&& c->getHP() == 0)
				continue;

		
			int d = privGetDistanceSquare(c, currentSelected, 1, 2);
			if (d < distance)
			{
				candidate = c;
				distance = d;
			}
		}
		if (candidate != 0)
			currentSelected = candidate;
	}

	if (Input::isPressed(Button_Left))
	{
		mySelectTargetState = SelectTarget_Enemies;
		std::vector<Character*> characters;
		characters = myBattle->getEnemyCharacters();

		int distance = 100000000;
		Character* candidate = 0;

		for (unsigned int i = 0; i < characters.size(); i++)
		{
			Character* c = characters[i];

			if (c->getMarkerX() >= currentSelected->getMarkerX())
				continue;

			if (mySelectedAction.getTargetPolicy() == ActionTrigger::TargetPolicy_OnlyAlive
				&& c->getHP() == 0)
				continue;

			
			int d = privGetDistanceSquare(c, currentSelected, 2, 1);
			if (d < distance)
			{
				candidate = c;
				distance = d;
			}
		}

		if (candidate != 0)
		{
			currentSelected = candidate;
		}
		else
		{
			mySelectTargetState = SelectTarget_Players;
		}
	}

	if (Input::isPressed(Button_Right))
	{
		if (mySelectTargetState == SelectTarget_Players)
			return;
	
		std::vector<Character*> characters;
		characters = myBattle->getEnemyCharacters();

		int distance = 100000000;
		Character* candidate = 0;

		for (unsigned int i = 0; i < characters.size(); i++)
		{
			Character* c = characters[i];

			if (c->getMarkerX() <= currentSelected->getMarkerX())
				continue;

			if (mySelectedAction.getTargetPolicy() == ActionTrigger::TargetPolicy_OnlyAlive
				&& c->getHP() == 0)
				continue;

			int d = privGetDistanceSquare(c, currentSelected, 2, 1);
			if (d < distance)
			{
				candidate = c;
				distance = d;
			}
		}

		if (candidate != 0)
		{
			currentSelected = candidate;
		}
		else // Couldn't select any enemies, move to the players
		{
			mySelectTargetState = SelectTarget_Players;
			characters = myBattle->getFightingPlayerCharacters();

			int distance = 100000000;

			for (unsigned int i = 0; i < characters.size(); i++)
			{
				Character* c = characters[i];
			
				if (c->getMarkerX() <= currentSelected->getMarkerX())
					continue;

				if (mySelectedAction.getTargetPolicy() == ActionTrigger::TargetPolicy_OnlyAlive
					&& c->getHP() == 0)
					continue;

				int d = privGetDistanceSquare(c, currentSelected, 1, 2);
				if (d < distance)
				{
					candidate = c;
					distance = d;
				}
			}

			if (candidate != 0)
			{
				currentSelected = candidate;
			}
			else
			{
				mySelectTargetState = SelectTarget_Enemies;
			}
		}
	}

	if (oldSelected != currentSelected)
	{
		Sound::playSample("data/sound/gui_select.wav");
	}
	mySelectedTargets.push_back(currentSelected);
}

void Gui::privLogicSelectTargetAction()
{
	if (ScreenManager::getTop() != this)
	{
		return;
	}

	if (Input::isPressed(Button_Action))
	{
		if (mySelectedTargets.size() != 0)
		{
			Action* action = ActionFactory::createAction(mySelectedAction, myPlayerCharacterWithAction, mySelectedTargets);
			myBattle->enqueueAction(action);
			if (mySelectedAction.getType() == ActionTrigger::Type_Spell)
			{
				myPlayerCharacterWithAction->enterPrepareCastState();
			}
			mySelectTargetState = SelectTarget_None;
		}

		mySelectedTargets.clear();
		privResetSelectAction();
	}
}

int Gui::privGetDistanceSquare( Character* aCharacter1, Character* aCharacter2, int aXScale, int aYScale) const
{
	int x = (aCharacter1->getMarkerX() - aCharacter2->getMarkerX()) * aXScale;
	int y = (aCharacter1->getMarkerY() - aCharacter2->getMarkerY()) * aYScale;
	return x*x + y*y;
}

void Gui::privDoSelectActionAttack()
{
	mySelectActionDone = true;
	mySelectedAction = ActionTriggerLibrary::find("Attack");
	privSwitchToSelectTarget();
}

void Gui::privDoSelectActionSpecial()
{
	mySelectActionDone = true;
	mySelectedAction = (*myPlayerCharacterWithAction->getSpecialActionTriggers().begin());
	privSwitchToSelectTarget();
}

void Gui::privDoSelectActionMagic( int aIndex )
{
	int i = 0;
	std::set<ActionTrigger> spells = myPlayerCharacterWithAction->getSpellActionTriggers();
	for (std::set<ActionTrigger>::iterator it = spells.begin(); it != spells.end(); it++)
	{
		if (i == aIndex)
		{
			mySelectedAction = (*it);

			if (mySelectedAction.getMPCost() > myPlayerCharacterWithAction->getMP())
			{
				return;
			}

			mySelectActionDone = true;

			
			break;
		}
		i++;
	}

	privSwitchToSelectTarget();
}

bool Gui::isDialogueDone() const
{
	return myDialogueDone;
}

void Gui::privDrawEnemyInfo( BITMAP* buffer )
{
	std::vector<Character*> enemies = myBattle->getAliveEnemyCharacters();

	for (unsigned int i = 0; i < enemies.size(); i++)
	{
		Character* c = enemies[i];
		myFont->draw(buffer, c->getName(), 10, 168 + myFont->getHeight() * i);
	}
}

void Gui::setInteractable( bool interactable )
{
	myInteractable = interactable;
}	
