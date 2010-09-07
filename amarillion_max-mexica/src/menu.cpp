#include "menu.h"
#include "color.h"
#include "engine.h"
#include <alfont.h>
#include "settings.h"
#include "game.h"

Menu::Menu(Engine & e) : Layer (e), selected (0), tFlash(0), mMain(), mKeys(), current(&mMain)
{
	
}

Menu::~ Menu() 
{
}

void Menu::draw(BITMAP * buffer)
{
	int y = 60;
	int dy = (buffer->h - 160) / current->size();
	for (int i = 0; i < current->size(); ++i)
	{
		int color = (*current)[i]->getColor();
		if ((*current)[i]->isFlashing())
		{
			if ((tFlash % 10) < 5) color = GREEN;
		}
		else if (i == selected && (tFlash % 30) < 15) color = RED;
		alfont_textout_centre_aa_ex (buffer, menufont, (*current)[i]->getText().c_str(), buffer->w / 2, y, 
			color, -1);
		y += dy;
	}
	
	alfont_textout_centre_aa_ex (buffer, menufont, (*current)[selected]->getHint().c_str(), buffer->w / 2, 
		buffer->h - 80, WHITE, -1);
}

void Menu::update()
{
	tFlash++;
}

void Menu::handleInput()
{
	int action = (*current)[selected]->handleInput();
	if (action != MENU_NONE)
	{
		handleAction (action);
	}
}

void Menu::handleAction(int action)
{
	switch (action)
	{
		case MENU_START:
			parent.setEvent (Engine::E_STARTGAME);
		break;
		case MENU_SETTINGS:
			current = &mKeys;
			selected = 0;
			tFlash = 0;
		break;
		case MENU_BACK:
			current = &mMain;
			selected = 0;
			tFlash = 0;
		break;
		case MENU_QUIT:
			parent.setEvent (Engine::E_QUIT);
		break;
		case MENU_RESUME:
			parent.setEvent (Engine::E_RESUME);
		break;
		case MENU_STOPGAME:
			parent.setEvent (Engine::E_MAINMENU);
		break;
		case MENU_NONE:
			// do nothing.
		break;
		case MENU_REPLAY:
			current = &mStartReplays;
			selected = 0;
			tFlash = 0;
		break;
		case MENU_START_REPLAY:
			parent.setEvent (Engine::E_STARTREPLAY);
		break;
		case MENU_SAVE_REPLAY:
			parent.saveReplay(selectedReplay);
			parent.setEvent (Engine::E_MAINMENU);
		break;
		case MENU_VIEWCODE:
			parent.setEvent (Engine::E_VIEWCODE);
		break;
	}
}

void Menu::init()
{
	menufont = parent.getResources()->getAlfont("Olivia1_40");

	mMain.add (new ActionMenuItem(this, MENU_START, "Start", "Start a new game"));
	mMain.add (new ActionMenuItem(this, MENU_VIEWCODE, "View Code", "View the source code of this game"));
	mMain.add (new ActionMenuItem(this, MENU_REPLAY, "Load replay", "Load a replay"));
	mMain.add (new ActionMenuItem(this, MENU_SETTINGS, "Settings", "Configure keys and other settings"));
	mMain.add (new ActionMenuItem(this, MENU_QUIT, "Quit", "Exit game"));
	mKeys.add (new KeyMenuItem(this, "Up", config_keys[btnUp], parent.getButton()[btnUp]));
	mKeys.add (new KeyMenuItem(this, "Down", config_keys[btnDown], parent.getButton()[btnDown]));
	mKeys.add (new KeyMenuItem(this, "Left", config_keys[btnLeft], parent.getButton()[btnLeft]));
	mKeys.add (new KeyMenuItem(this, "Right", config_keys[btnRight], parent.getButton()[btnRight]));
	mKeys.add (new KeyMenuItem(this, "Action", config_keys[btnAction], parent.getButton()[btnAction]));
	mKeys.add (new ActionMenuItem(this, MENU_BACK, "Main Menu", "Return to the main menu"));	
	mPause.add (new ActionMenuItem (this, MENU_RESUME, "Resume", "Resume game"));
	mPause.add (new ActionMenuItem (this, MENU_STOPGAME, 
		"Exit to Main Menu", "Stop game and exit to main menu"));

	for (int i = 0; i < 5; ++i)
	{
		mStartReplays.add (new ReplayMenuItem (this, MENU_START_REPLAY, i));
		mSaveReplays.add (new ReplayMenuItem (this, MENU_SAVE_REPLAY, i));
	}
	mStartReplays.add (new ActionMenuItem(this, MENU_BACK, "Main Menu", "Return to the main menu"));
	mSaveReplays.add (new ActionMenuItem(this, MENU_BACK, "Main Menu", "Return to the main menu"));
}

MenuList::~MenuList()
{
	for (vector<MenuItem*>::iterator i = items.begin(); i != items.end(); ++i)
	{
		delete (*i);
	}
	items.clear();
}

int KeyMenuItem::handleInput()
{
	if (waitForKey)
	{
		int c = getLastScancode();
		if (c != 0)
		{	
			clear_keybuf();
			if (c == KEY_ESC)
			{
			}
			else
			{				
				btn.setScancode(c);
				set_config_int ("tins10", btnConfigName, c);
			}
			waitForKey = false;
			flashing = false;
		}
	}
	else
	{
		if (keypressed())
		{
			int c = readkey();
			switch (c >> 8)
			{
				case KEY_LEFT:
				case KEY_UP:
					parent->prev();
					break;
				case KEY_DOWN:
				case KEY_RIGHT:
					parent->next();
					break;
				case KEY_ENTER:
				case KEY_SPACE:
					resetLastScancode();
					waitForKey = true;
					flashing = true;
					break;
			}
		}	
	}
	
	return Menu::MENU_NONE;
}

string KeyMenuItem::getText()
{
	return string (btnName + " [" + btn.name() + "]");
}

string KeyMenuItem::getHint()
{
	if (waitForKey)
		return string ("press a new key or esc to cancel");
	else
		return string ("press enter to change key");
}

int ActionMenuItem::handleInput()
{
	if (keypressed())
	{
		int c = readkey();
		switch (c >> 8)
		{
			case KEY_LEFT:
			case KEY_UP:
				parent->prev();
				break;
			case KEY_DOWN:
			case KEY_RIGHT:
				parent->next();
				break;
			case KEY_ENTER:
			case KEY_SPACE:
				return action;
				break;
		}
	}
	return Menu::MENU_NONE;
}

void Menu::prev()
{
	if (--selected < 0) selected = current->size() - 1;
	tFlash = 0;
}

void Menu::next()
{
	if (++selected >= current->size()) selected = 0;
	tFlash = 0;
}

ReplayMenuItem::ReplayMenuItem(Menu *_parent, int _action, int _index) :
		MenuItem (_parent), action (_action), index (_index)
{
	char buf[128];
	snprintf (buf, sizeof(buf), "Replay %i", index+1);
	text = string(buf);
	if (action == Menu::MENU_START_REPLAY)
	{
		snprintf (buf, sizeof(buf), "Start replay of slot %i", index+1);
	}
	else if (action == Menu::MENU_SAVE_REPLAY)
	{
		snprintf (buf, sizeof(buf), "Save replay to slot %i", index+1);
	}
	hint = string(buf);
}

int ReplayMenuItem::handleInput()
{
	if (keypressed())
	{
		int c = readkey();
		switch (c >> 8)
		{
			case KEY_LEFT:
			case KEY_UP:
				parent->prev();
				break;
			case KEY_DOWN:
			case KEY_RIGHT:
				parent->next();
				break;
			case KEY_ENTER:
			case KEY_SPACE:
				if (action == Menu::MENU_SAVE_REPLAY || replayFileExists(index))
				{
					parent->selectReplay(index);
					return action;
				}
				break;
		}
	}
	return Menu::MENU_NONE;
}

int ReplayMenuItem::getColor()
{
	return replayFileExists(index) ? YELLOW : GREY;
}
