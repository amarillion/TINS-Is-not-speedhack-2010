#pragma once
#include "Character.hpp"
#include "Animation.hpp"

class ActionTrigger;

class PlayerCharacter : public Character {
public:
	PlayerCharacter(const std::string &name, int homeX, int homeY, int level, int hp, int mp, float speed);

	bool isFighting();

	void setFighting(bool fighting);

	void onRender(BITMAP *dest);
	void onLogic();

	bool isPrepareCastStateEntered();
	void enterPrepareCastState();
	void enterIdleState();
	bool isIdleStateEntered();
	void enterCastState();
	bool isCastStateEntered();
	void enterDeadState();
	void enterDamageState();
	void enterAttackState();
	bool isAttackStateEntered();
	void enterHideState();
	bool isHideStateEntered();
	bool isDead();

private:
	bool myFighting;
	Animation myIdleAnimation;
	Animation myDeadAnimation;
	Animation myDamageAnimation;
	Animation myWalkAnimation;
	Animation myCastAnimation;
	Animation myChannelAnimation;
	Animation myAttackAnimation;

	enum State
	{
		State_EnterIdle,
		State_Idle,
		State_PrepareCast,
		State_Cast,
		State_EnterCast,
		State_EnterAttack,
		State_DoAttack,
		State_Attack,
		State_Damage,
		State_EnterHide,
		State_Hide,
		State_Dead
	};

	State myState;
	int myFrameCounter;
	int myTime;
};