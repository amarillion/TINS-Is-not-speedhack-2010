#include "Precompiled.hpp"
#include "Util.hpp"
#include "ActionTriggerLibrary.hpp"
#include "Character.hpp"
#include "Time.hpp"
#include "Action.hpp"
#include "BouncyTextEntity.hpp"
#include "Battle.hpp"

Character::Character(Faction faction, const std::string &name, int homeX, int homeY, int level, int hp, int mp, float speed)
	: myFaction(faction)
	, myName(name)
	, myHomeX(homeX)
	, myX(homeX)
	, myHomeY(homeY)
	, myY(homeY)
	, myLevel(level)
	, myHP(hp)
	, myMaxHP(hp)
	, myMP(mp)
	, myMaxMP(mp)
	, myTime(0.0)
	, mySpeed(speed)
	, myQueuedAction(NULL)
	, myDying(false)
{
}

Character::~Character()
{
}

std::string Character::getName()
{
	return myName;
}

int Character::getMarkerX()
{
	return getX();
}

int Character::getMarkerY()
{
	return getY() - myHeight;
}

void Character::onLogic()
{
	myTime += mySpeed / Time::TicksPerSecond;
	myTime = clamp(myTime, 0.0f, 1.0f);
}

void Character::onRender( BITMAP *dest )
{
	// TODO
}

float Character::getZOrder()
{
	return myHomeY;
}

bool Character::isToBeDestroyed()
{
	return myDying && isDeadStateEntered();
}

int Character::getHP()
{
	return myHP;
}

void Character::setHP( int hp )
{
	myHP = clamp(hp, 0, myMaxHP);
}

int Character::getMaxHP()
{
	return myMaxHP;
}

void Character::setMaxHP( int maxHP )
{
	myMaxHP = maxHP;
	myHP = clamp(myHP, 0, myMaxHP);
}

void Character::adjustHP( int delta )
{
	std::string pointsAsText = "N/A";
	int x = getX(), y = getY();
	unsigned int color;

	if(delta < 0) {
		pointsAsText = toString(-delta);
		color = makecol(255, 20, 20);
	} else if( delta > 0) {
		pointsAsText = toString(delta);
		color = makecol(20, 255, 20);
	}

	myBattle->addEntity(new BouncyTextEntity(pointsAsText, x, y, color, this->getZOrder()));

	setHP(getHP() + delta);
}

int Character::getMP()
{
	return myMP;
}

void Character::setMP( int mp )
{
	myMP = clamp(mp, 0, myMaxMP);
}

int Character::getMaxMP()
{
	return myMaxMP;
}

void Character::setMaxMP( int maxMP )
{
	myMaxMP = maxMP;
	myMP = clamp(myMP, 0, myMaxMP);
}

void Character::adjustMP( int delta )
{
	setMP(getMP() + delta);
}

int Character::getLevel()
{
	return myLevel;
}

void Character::setLevel( int level )
{
	myLevel = level;
}

int Character::damage( int amount, Damage type )
{
	amount = amount * getDamageSensitivity(type);
	adjustHP(-amount);
	enterDamageState();

	return amount;
}

float Character::getDamageSensitivity( Damage type )
{
	switch(type) {
		case Damage_Cure:
			return -1.0f;
		default:
			return 1.0;
	}
}

float Character::getSpeed()
{
	return mySpeed;
}

void Character::setSpeed( float speed )
{
	mySpeed = speed;
}

float Character::getTime()
{
	return myTime;
}

bool Character::isReadyForAction()
{
	return myTime >= 1.0 && myQueuedAction == NULL;
}

void Character::addActionTrigger( const std::string& name )
{
	myActionTriggers.insert(ActionTriggerLibrary::find(name));
}

const std::set<ActionTrigger> Character::getActionTriggers()
{
	return myActionTriggers;
}

bool Character::isDead()
{
	return myDying;
}

void Character::setQueuedAction( Action *action )
{
	if (action == NULL)
	{
		myTime = 0.0;
	}

	myQueuedAction = action;
}

Character::Faction Character::getFaction()
{
	return myFaction;
}

const std::set<ActionTrigger> Character::getSpellActionTriggers()
{
	std::set<ActionTrigger> result;

	for (std::set<ActionTrigger>::iterator it = myActionTriggers.begin(); it != myActionTriggers.end(); it++)
	{
		ActionTrigger trigger = *it;
		if (trigger.getType() == ActionTrigger::Type_Spell)
		{
			result.insert(trigger);
		}
	}

	return result;
}

const std::set<ActionTrigger> Character::getSpecialActionTriggers()
{
	std::set<ActionTrigger> result;

	for (std::set<ActionTrigger>::iterator it = myActionTriggers.begin(); it != myActionTriggers.end(); it++)
	{
		ActionTrigger trigger = *it;
		if (trigger.getType() == ActionTrigger::Type_Ability)
		{
			result.insert(trigger);
		}
	}

	return result;
}

int Character::getX()
{
	return myX;
}

int Character::getY()
{
	return myY;
}

void Character::setBattle( Battle* battle )
{
	myBattle = battle;
}

void Character::setHeight( int aHeight )
{
	myHeight = aHeight;
}

int Character::getHeight()
{
	return myHeight;
}

int Character::getCenterX()
{
	return myX;
}

int Character::getCenterY()
{
	return myY - myHeight / 2;
}

void Character::setHome( int x, int y )
{
	myHomeX = x;
	myHomeY = y;
	
}

void Character::setPosition( int x, int y )
{
	myX = x;
	myY = y;
}
