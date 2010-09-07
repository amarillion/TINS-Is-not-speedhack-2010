#include <assert.h>
#include "button.h"
#include <allegro.h>

bool keyboardInited = false;

volatile int lastScancode; // last keypress, not release

void keyboard_handler (int scancode)
{
	if ((scancode & 128) == 0) lastScancode = scancode;
}
END_OF_FUNCTION(keyboard_handler);

void initKeyboard ()
{
	LOCK_VARIABLE(last_scancode);
	LOCK_FUNCTION(keyboard_handler);
	lastScancode = 0;
	keyboard_lowlevel_callback = keyboard_handler;
	keyboardInited = true;
}

void resetLastScancode()
{
	lastScancode = 0;
}

int getLastScancode()
{
	return lastScancode;
}

bool Button::getState()
{ 
	return key[scancode] || key[altcode]; 
}

bool Button::justPressed()
{
	bool newState = getState();
	bool result = (newState && !prevState);
	prevState = newState;
	return result;
}

const char *Button::name()
{ 
	return scancode_to_name(scancode);
}
