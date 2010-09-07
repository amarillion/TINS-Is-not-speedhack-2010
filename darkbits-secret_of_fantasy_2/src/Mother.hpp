#pragma once

#include "PlayerCharacter.hpp"

class Mother: public PlayerCharacter
{
public:
	Mother(int homeX, int homeY);
	void remove();
	bool isToBeDestroyed();

private:
	bool myRemoveMyself;
};