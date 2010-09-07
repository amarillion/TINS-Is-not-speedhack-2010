#pragma 

#include "Screen.hpp"
#include "Animation.hpp"
#include "Dialogue.hpp"
#include "ActionTrigger.hpp"

class GuiDialogue;
class Character;
class Battle;
class Font;
class PlayerCharacter;

class Gui : public Screen
{
public:
	Gui(Battle* aBattle);

	void onDraw(BITMAP* buffer);
	void onLogic();

	void showMessage(const std::string& aMessage);
	void showDialogue(const Dialogue& aDialogue);
	bool isDialogueDone() const;
	void setInteractable(bool interactable);

private:
	void privLogicDialogue();
	
	void privLogicTitle();
	void privDrawTitle( BITMAP* buffer );
	
	void privLogicPlayerInfo();
	void privDrawPlayerInfo(BITMAP* buffer);
	void privDrawPlayerInfo(BITMAP* buffer, Character* aCharacter, int aYOrder);
	
	void privDrawEnemyInfo(BITMAP* buffer);

	void privResetSelectAction();
	void privSwitchToSelectAction(bool aRemberLastPosition);
	void privLogicSelectAction();
	void privDrawSelectAction(BITMAP* buffer);

	void privSwitchToSelectActionMain(bool aRememberLast);
	void privLogicSelectActionMain();
	void privDrawSelectActionMain(BITMAP* buffer);
	
	void privSwitchToSelectActionSecondary();
	void privLogicSelectActionSecondary();
	void privDrawSelectActionSecondary(BITMAP* buffer);

	void privSwitchToSelectActionMagic();
	void privLogicSelectActionMagic();
	void privDrawSelectActionMagic(BITMAP* buffer);
	
	void privSwitchToSelectActionItem();
	void privLogicSelectActionItem();
	void privDrawSelectActionItem(BITMAP* buffer);

	void privSwitchToSelectTarget();
	void privLogicSelectTarget();
	void privLogicSelectTargetCheckSelected();
	void privLogicSelectTargetAll();
	void privLogicSelectTargetMakeSureSomethingIsSelected();
	void privLogicSelectTargetSingle();
	void privLogicSelectTargetAction();
	void privDrawSelectTarget(BITMAP* buffer);
	
	void privDoSelectActionAttack();
	void privDoSelectActionSpecial();
	void privDoSelectActionMagic(int aIndex);
	
	int privGetDistanceSquare(Character* aCharacter1, Character* aCharacter2, int aXScale, int aYScale) const;
	
	enum SelectActionFocus
	{
		SelectActionFocus_None,
		SelectActionFocus_Main,
		SelectActionFocus_Secondary,
		SelectActionFocus_Magic,
		SelectActionFocus_Item
	};
	SelectActionFocus mySelectActionFocus;
	bool mySelectAction;
	bool mySelectActionDone;
	ActionTrigger mySelectedAction;
	
	int mySelectActionMainRow;
	int mySelectMainFrameCounter;
	
	int mySelectActionMagicFrameCounter;
	int mySelectActionMagicScroll;
	int mySelectActionMagicHandRow;
	int mySelectActionMagicRow;
	bool mySelectActionMagicNextFrame;

	int mySelectActionSecondaryRow;
	int mySelectSecondaryFrameCounter;

	enum SelectTargetState
	{
		SelectTarget_None,
		SelectTarget_Enemies,
		SelectTarget_Players
	};
	bool mySelectTargetNextFrame;
	bool mySelectTargetAll;
	int mySelectTargetFrameCounter;
	SelectTargetState mySelectTargetState;
	std::vector<Character*> mySelectedTargets;

	std::string myInfoMessageLeft;
	std::string myInfoMessageRight;

	PlayerCharacter* myPlayerCharacterWithAction;
	Battle* myBattle;
	std::string myTitle;
	int myTitleCounter;
	Font* myFont;
	Font* myFontDisabled;
	Font* myHighlightedFont;
	Animation myMessageBackground;
	Animation myInfoMessageBackground;
	Animation myBackground;
	Animation myActionBackground;
	Animation myCharacterTimer;
	Animation myHand;
	Animation myMarker;
	Animation myArrowRight;
	Animation myArrowLeft;
	bool myShowTitle;
	bool myDialogueDone;
	GuiDialogue* myGuiDialogue;
	int myCounter;
	bool myInteractable;
};