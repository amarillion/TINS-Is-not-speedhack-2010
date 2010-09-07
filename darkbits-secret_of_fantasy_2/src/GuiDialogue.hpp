#pragma once

#include "Screen.hpp"
#include "Animation.hpp"
#include "Dialogue.hpp"
#include "Font.hpp"

class GuiDialogue: public Screen
{
public:
	GuiDialogue(const Dialogue& aDialogue);
	void onDraw(BITMAP* buffer);
	void onLogic();
	bool isDone() const;

private:
	int myCurrentText;
	Animation myBackground;
	Font* myFont;
	Dialogue myDialogue;
	unsigned int myCurrentNumberOfCharacters;
	int myCounter;
	bool myFastForward;
	bool myIsDone;
};