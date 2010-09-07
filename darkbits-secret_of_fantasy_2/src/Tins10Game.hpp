#pragma once

#include "Game.hpp"

class Fight;
class Battle;
class Tins10Game: public Game
{
public:
	Tins10Game();

	
protected:
	void onLogic();
private:
	Fight *createFight1();
	Fight *createFight2();
};