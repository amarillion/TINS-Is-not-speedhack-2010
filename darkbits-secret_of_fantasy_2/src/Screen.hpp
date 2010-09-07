#pragma once 

#include <allegro.h>

class Screen
{
public:
	enum State
	{
		Active,
		Hidden
	};

	Screen(bool dialog);
	virtual ~Screen() {}
	
	virtual void onLogic() = 0;
	virtual void onDraw(BITMAP* buffer) = 0;
	
	State getState();
	void setState(State state);
	bool isActive();
	bool isTop();
	void exit();
	bool isDialog();
	void hide();

private:
	State mState;
	bool mDialog;
};