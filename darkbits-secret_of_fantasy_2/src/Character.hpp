#pragma once

#include "Entity.hpp"
#include "ActionTrigger.hpp"

class Action;
class Battle;
class Font;

class Character : public Entity {
public:
	enum Damage {
		Damage_Physical,
		Damage_Fire,
		Damage_Ice,
		Damage_Lighting,
		Damage_Cure,
		Damage_Hack
	};

	enum Faction {
		Faction_Player,
		Faction_Enemy,
		Faction_None,
	};

	void setBattle(Battle* battle);

protected:
	Character(Faction faction, const std::string &name, int homeX, int homeY, int level, int hp, int mp, float speed);
	void setHeight(int aHeight);
public:
	virtual ~Character();

	virtual bool isPrepareCastStateEntered() { return true;}
	virtual void enterPrepareCastState() {}
	virtual void enterIdleState() {}
	virtual bool isIdleStateEntered() { return true;}
	virtual void enterCastState() {}
	virtual bool isCastStateEntered() { return true; }
	virtual void enterDeadState() { myDying = true; }
	virtual bool isDeadStateEntered() { return true; }
	virtual void enterDamageState() {}
	virtual void enterAttackState() {}
	virtual bool isAttackStateEntered() { return true; }
	virtual void enterHideState() {}
	virtual bool isHideStateEntered() { return true; }
	
	virtual Faction getFaction();

	virtual std::string getName();

	virtual int getMarkerX();

	virtual int getMarkerY();

	virtual void onLogic();

	virtual void onRender(BITMAP *dest);

	virtual float getZOrder();

	virtual bool isToBeDestroyed();

	virtual int getX();

	virtual int getY();

	virtual bool isDead();

	virtual int getHP();

	virtual void setHP(int hp);

	virtual int getMaxHP();

	virtual void setMaxHP(int maxHP);

	virtual void adjustHP(int delta);

	virtual int getMP();

	virtual void setMP(int mp);

	virtual int getMaxMP();

	virtual void setMaxMP(int maxMP);

	virtual void adjustMP(int delta);

	virtual int getLevel();

	virtual void setLevel(int level);

	virtual float getSpeed();

	virtual void setSpeed(float speed);

	virtual float getTime();

	virtual bool isReadyForAction();


	virtual float getDamageSensitivity(Damage type);

	virtual int damage(int amount, Damage type);

	
	void addActionTrigger(const std::string& name);

	const std::set<ActionTrigger> getActionTriggers();

	const std::set<ActionTrigger> getSpellActionTriggers();

	const std::set<ActionTrigger> getSpecialActionTriggers();

	void setQueuedAction(Action *action);

	virtual void flee() {};

	int getHeight();

	int getCenterX();
	int getCenterY();

	void setHome(int x, int y);
	void setPosition(int x, int y);

protected:
	Faction myFaction;
	std::string myName;
	int myX, myY;
	int myHomeX, myHomeY;
	int myHeight;
	int myHP, myMaxHP, myMP, myMaxMP;
	int myLevel;
	float mySpeed;
	float myTime;
	Action *myQueuedAction;
	Battle *myBattle;
	bool myDying;

	std::set<ActionTrigger> myActionTriggers;
};
