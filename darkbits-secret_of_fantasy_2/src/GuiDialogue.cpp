#include "Precompiled.hpp"
#include "GuiDialogue.hpp"
#include "Resource.hpp"
#include "Input.hpp"
#include "Constants.hpp"
#include "ScreenManager.hpp"

GuiDialogue::GuiDialogue( const Dialogue& aDialogue )
:Screen(true)
, myDialogue(aDialogue)
, myBackground("data/images/gui_dialogue_background.bmp", 1)
, myCurrentText(0)
,myCurrentNumberOfCharacters(1)
,myCounter(0)
,myFastForward(false)
,myIsDone(false)
{
	myFont = Resource::getFont("data/images/font.bmp");
}

void GuiDialogue::onDraw( BITMAP* buffer )
{
	if (myIsDone)
		return;
	myBackground.drawFrame(buffer, 0, 0, 161);
	myFont->drawWrap(buffer, myDialogue.getText(myCurrentText), 8, 168, 308, myCurrentNumberOfCharacters);
}

void GuiDialogue::onLogic()
{
	if (myIsDone)
		return;

	myCounter++;

	if (Input::isHeld(Button_Action))
	{
		myFastForward = true;
	}

	if (myCurrentNumberOfCharacters == myDialogue.getText(myCurrentText).size())
	{
		if (Input::isPressed(Button_Action))
		{
			myCurrentText++;
			myCurrentNumberOfCharacters = 1;
			myCounter = 0;
			myFastForward = false;
			if (myCurrentText == myDialogue.getNumberOfTexts())
			{
				myIsDone = true;
				return;
			}
		}

		return;
	}

	if (Input::isHeld(Button_Action) && myFastForward)
	{
		myCurrentNumberOfCharacters += 2;
	}
	else
	{
		if (myCounter % 4 == 0)
		{
			myCurrentNumberOfCharacters++;
		}
	}	

	if (myCurrentNumberOfCharacters > myDialogue.getText(myCurrentText).size())
	{
		myCurrentNumberOfCharacters = myDialogue.getText(myCurrentText).size();
	}
}

bool GuiDialogue::isDone() const
{
	return myIsDone;
}
