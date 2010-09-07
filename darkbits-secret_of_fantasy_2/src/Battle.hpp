#pragma once

#include "Screen.hpp"

class BattleMonitor;
class Entity;
class Character;
class PlayerCharacter;
class EnemyCharacter;
class Gui;
class Action;
class GameOverScreen;
class Fight;
class Font;

class Battle : public Screen
{
public:
	enum BattleState {
		State_Battle,
		State_PreGameOver,
		State_GameOver
		
	};
	Battle();
	
	~Battle();
	
	void onLogic();
	
	void onDraw(BITMAP* buffer);

	Character *findCharacter(const std::string name);

	const std::vector<PlayerCharacter *> &getAllPlayerCharacters();

	std::vector<Character *> getFightingPlayerCharacters();
	std::vector<Character *> getAliveFightingPlayerCharacters();
	std::vector<Character *> getEnemyCharacters();
	std::vector<Character *> getAliveEnemyCharacters();

	PlayerCharacter* getPlayerCharacterWithActionChoice() const;
	void placePlayerWithActionChoiceLastInQueue();

	void addEntity(Entity *entity);

	Gui* getGui() const;

	void enqueueAction(Action *action, bool topPriority = false);

	bool isOnBattlefield(Character *character);

	void enqueueFight(Fight* fight);
	bool isAllEnemiesDead();

private:
	void privGameDraw(BITMAP* buffer);
	void privGameOverDraw(BITMAP* buffer);

	void privUpdateBattleMonitors();
	void privUpdateActions();

	void privPreGameOverLogic();

	bool privIsFighting();
	bool privIsNewFightAvailable();
	bool privIsAllFiightingPlayersDead();
	bool privCheckShouldFightChange();
	bool privCheckShouldEnqueueFightOutroActions();
	bool privCheckShouldEnterPreGameOver();

	void privEnterNewFight();
	void privEnterPreGameOver();
	void privEnqueueOutroActions();
	void privEnemyAndOtherEntitiesLogic();

	void privPlayerCharacterLogic();
	bool privIsPlayerCharacterInActionQueue(PlayerCharacter* aPlayerCharacter);
	void Battle::privCleanUpDeadEnemies();

	Gui* myGui;
	std::vector<Entity *> myOtherEntities;
	std::vector<PlayerCharacter *> myPlayerCharacters;
	std::vector<EnemyCharacter *> myEnemyCharacters;
	std::deque<PlayerCharacter*> myPlayerCharacterActionQueue;
	PlayerCharacter* myPlayerCharacterWithActionChoice;
	std::list<Action *> myActionQueue;
	Action *myCurrentAction;
	BattleState myBattleState;
	GameOverScreen *myGameOverScreen;
	int myPreGameOverFrameCounter;
	bool myInCleanupPhase;
	std::deque<Fight *> myFights;
	Fight *myCurrentFight;
	bool myOutroActionsEnqueued;
	std::vector<BattleMonitor*> myBattleMonitors;
	Font* myFont;
};