#include "Precompiled.hpp"
#include "Screen.hpp"
#include "ScreenManager.hpp"

Screen::Screen(bool dialog)
:mDialog(dialog),
 mState(Screen::Hidden)
{

}

Screen::State Screen::getState()
{
	return mState;
}

void Screen::setState(State state)
{
	mState = state;
}

bool Screen::isActive()
{ 
	return ScreenManager::getActive() == this;
}

bool Screen::isTop()
{ 
	return  ScreenManager::getTop() == this;
}

void Screen::exit()
{
     ScreenManager::exit(this);
}

bool Screen::isDialog()
{
	return mDialog;
}

void Screen::hide()
{
	mState = Screen::Hidden;
}