#pragma once
#include "Character.hpp"
#include "Animation.hpp"

class EnemyCharacter : public Character 
{
public:
	EnemyCharacter(const std::string &name, const std::string &sprite, int homeX, int homeY, int level, int hp, int mp, float speed);

	void onRender(BITMAP* aBuffer);

	void onLogic();

	virtual void enterDeadState();

	virtual bool isDeadStateEntered();

	void enterCastState();

	void enterAttackState();

	void flee();

private:
	Animation myAnimation;
	int myDeathTime;
	int myDeathIsDone;
	int myCastTime;
};