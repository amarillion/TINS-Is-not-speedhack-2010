#include "Precompiled.hpp"
#include "PlayerCharacter.hpp"
#include "ActionTrigger.hpp"

PlayerCharacter::PlayerCharacter(const std::string &name,int homeX, int homeY, int level, int hp, int mp, float speed ) 
	: Character(Faction_Player, name, homeX, homeY, level, hp, mp, speed)
	, myFighting(true)
	, myAttackAnimation("data/images/characters/" + name + "/attack.bmp")
	, myIdleAnimation("data/images/characters/" + name + "/idle.bmp")
	, myWalkAnimation("data/images/characters/" + name + "/walk.bmp")
	, myCastAnimation("data/images/characters/" + name + "/cast.bmp")
	, myDamageAnimation("data/images/characters/" + name + "/damage.bmp")
	, myDeadAnimation("data/images/characters/" + name + "/dead.bmp")
	, myChannelAnimation("data/images/characters/" + name + "/channel.bmp")
	, myState(State_Idle)
{
	setHeight(32);
}

bool PlayerCharacter::isFighting()
{
	return myFighting;
}

void PlayerCharacter::setFighting( bool fighting )
{
	myFighting = fighting;
}

void PlayerCharacter::onRender( BITMAP *dest )
{
	int x = getX() - 16;
	int y = getY() - 32;

	if (myState == State_Idle)
	{
		myIdleAnimation.drawFrame(dest, 0, x, y);
	}
	if (myState == State_EnterIdle)
	{
		myWalkAnimation.drawFrame(dest, myFrameCounter / 4,x, y);
	}
	if (myState == State_EnterAttack)
	{
		myIdleAnimation.drawFrame(dest, 0, x, y);
	}
	if (myState == State_DoAttack)
	{
		myAttackAnimation.drawFrame(dest, 0, x, y);
	}
	if (myState == State_Attack)
	{
		myAttackAnimation.drawFrame(dest, 0, x, y);
	}
	if (myState == State_EnterCast)
	{
		myWalkAnimation.drawFrame(dest, myFrameCounter / 4, x, y);
	}
	if (myState == State_EnterHide)
	{
		myWalkAnimation.drawFrame(dest, myFrameCounter / 4, x, y, true);
	}
	if (myState == State_PrepareCast)
	{
		myChannelAnimation.drawFrame(dest, myFrameCounter / 10, x, y);
	}
	if (myState == State_Cast)
	{
		myCastAnimation.drawFrame(dest, 0, x, y);
	}
	if (myState == State_Damage)
	{
		Blending b = Blending_None;
		if (myTime < 6)
		{
			b = Blending_Lighten;
		}

		myDamageAnimation.drawFrame(dest, 0, x, y, false, false, b);
	}
	if (myState == State_Dead)
	{
		myDeadAnimation.drawFrame(dest, 0, x, y, false, false);
	}

	//circle(dest, getX(), getY(), 5, makecol(255, 255, 255));
}

void PlayerCharacter::enterIdleState()
{
	myState = State_EnterIdle;
}

bool PlayerCharacter::isIdleStateEntered()
{
	return myState == State_Idle;
}

void PlayerCharacter::enterPrepareCastState()
{
	myState = State_PrepareCast;
}

void PlayerCharacter::onLogic()
{
	Character::onLogic();

	myFrameCounter++;

	if (myState == State_EnterCast)
	{
		myX-=2;
		if (myHomeX - 40 >= myX)
		{
			myX = myHomeX - 40;
			myState = State_Cast;
		}
	}

	if (myState == State_EnterIdle)
	{
		if (myX == myHomeX)
			myState = State_Idle;
		else if (myX < myHomeX)
		{
			myX+=2;
			if (myX >= myHomeX)
				myX = myHomeX;
		}
		else if (myX > myHomeX)
		{
			myX-=2;
			if (myX <= myHomeX)
				myX = myHomeX;
		}
	}

	if (myState == State_EnterAttack)
	{
		myX -= 10;
		myState = State_DoAttack;
	}

	if (myState == State_DoAttack)
	{
		myTime++;
		if (myTime > 30)
			myState = State_Attack;
	}

	if (myState == State_Damage)
	{
		myTime++;
		if (myTime > 20)
		{
			myState = State_Idle;
		}
	}

	if (myState == State_EnterHide)
	{
		myX++;
		if (myX > (340 + 16 + 1))
		{
			myState = State_Hide;
		}
	}
}

bool PlayerCharacter::isPrepareCastStateEntered()
{
	return myState == State_PrepareCast;
}

void PlayerCharacter::enterCastState()
{
	myState = State_EnterCast;
}

bool PlayerCharacter::isCastStateEntered()
{
	return myState == State_Cast;
}

bool PlayerCharacter::isAttackStateEntered()
{
	return myState == State_Attack;
}

void PlayerCharacter::enterAttackState()
{
	myState = State_EnterAttack;
	myTime = 0;
}

void PlayerCharacter::enterDamageState()
{
	myState = State_Damage;
	myTime = 0;
}

void PlayerCharacter::enterHideState()
{
	myState = State_EnterHide;
}

bool PlayerCharacter::isHideStateEntered()
{
	return myState == State_Hide;
}

bool PlayerCharacter::isDead()
{
	return myState == State_Dead;
}

void PlayerCharacter::enterDeadState()
{
	myState = State_Dead;
}
