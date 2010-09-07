#include "layer.h"
#include <string>
#include <vector>
#include "button.h"
#include "color.h"

#ifndef MENU_H
#define MENU_H

using namespace std;

struct ALFONT_FONT;

class Menu;
class Engine;

class MenuItem
{
	protected:
		Menu *parent;
		bool flashing;
	public:
		MenuItem (Menu *_parent) : parent (_parent), flashing (false) {}
		virtual int handleInput() = 0;
		virtual string getText() = 0;
		virtual string getHint() = 0;
		bool isFlashing() { return flashing; }
		virtual int getColor() { return YELLOW; }
};

class KeyMenuItem : public MenuItem
{
	private:
		string btnName;
		const char* btnConfigName;
		Button &btn;
		bool waitForKey;
	public:
		KeyMenuItem (Menu *parent, string _btnName, const char* _btnConfigName, Button & _btn) : 
			MenuItem (parent), btnName (_btnName), 
			btnConfigName(_btnConfigName), btn(_btn), waitForKey (false) {}
		virtual int handleInput();
		virtual string getText();
		virtual string getHint();
};

class ActionMenuItem : public MenuItem
{
	private:
		int action; // code returned from handleInput
		string text;
		string hint;
	public:
		ActionMenuItem (Menu *_parent, int _action, string _text, string _hint) : 
			MenuItem (_parent), action (_action), text (_text), hint (_hint) {}
		virtual int handleInput();
		virtual string getText() { return text; }
		virtual string getHint() { return hint; }
};

class ReplayMenuItem : public MenuItem
{
private:
	int index; // code returned from handleInput
	int action;
	string text;
	string hint;
public:
	ReplayMenuItem (Menu *_parent, int _action, int _index);
	virtual int handleInput();
	virtual string getText() { return text; }
	virtual string getHint() { return hint; }
	virtual int getColor();
};

class MenuList
{
		vector <MenuItem*> items;
	public:
		MenuList() : items() {}
		~MenuList();
		MenuItem *operator[](int index) { return items[index]; }
		void add(MenuItem *item) { items.push_back(item); }
		int size() { return items.size(); }
};

class Menu : public Layer
{
	private:
		int selected;
		int tFlash;
		
		int selectedReplay;

		MenuList mMain;
		MenuList mKeys;
		MenuList mPause;
		MenuList mStartReplays;
		MenuList mSaveReplays;

		MenuList *current;
		
		ALFONT_FONT *menufont;
	public:
		enum { MENU_NONE, MENU_START, MENU_SETTINGS, MENU_QUIT, MENU_BACK, MENU_STOPGAME, MENU_RESUME, MENU_VIEWCODE, MENU_REPLAY, MENU_START_REPLAY, MENU_SAVE_REPLAY };
		Menu (Engine &e);
		virtual ~Menu();
		virtual void init();
		void prepare (int i)
		{ 
			switch (i)
			{
			case 0: current = &mMain; break;
			case 1: current = &mPause; break;
			case 2: current = &mSaveReplays; break;
			}
			selected = 0;
		} 
		virtual void draw(BITMAP *buffer);
		void prev();
		void next();
		virtual void update();
		virtual void handleInput();
		void handleAction (int action);
		void selectReplay (int i) { selectedReplay = i; }
		int getSelectedReplay() { return selectedReplay; }
};

#endif
