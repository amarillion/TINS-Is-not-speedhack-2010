#include "Precompiled.hpp"
#include "Battle.hpp"
#include "ScreenManager.hpp"
#include "PlayerCharacter.hpp"
#include "EnemyCharacter.hpp"
#include "Gui.hpp"
#include "Action.hpp"
#include "Input.hpp"
#include "Constants.hpp"
#include "Robo.hpp"
#include "GameOverScreen.hpp"
#include "ActionRemoveDeadCreatures.hpp"
#include "Fight.hpp"
#include "Music.hpp"
#include "BattleMonitor.hpp"

Battle::Battle()
	: Screen(false)
	, myGui(0)
	, myPlayerCharacterWithActionChoice(0)
	, myCurrentAction(NULL)
	, myBattleState(State_Battle)
	, myGameOverScreen(NULL)
	, myInCleanupPhase(false)
	, myCurrentFight(NULL)
{
}

Battle::~Battle()
{
}

void Battle::onLogic()
{
	if (!myGui)
	{
		myGui = new Gui(this);
		ScreenManager::add(myGui);
	}

	if(myBattleState == State_GameOver) 
	{
		if (ScreenManager::getTop() == myGui)
		{
			ScreenManager::exit(myGui);
			return;
		}
		if (ScreenManager::getTop() == this)
		{
			ScreenManager::exit(this);
			return;
		}
		return;
	}
	if (myBattleState == State_PreGameOver)
	{
		privPreGameOverLogic();
	}

	if (privCheckShouldEnqueueFightOutroActions())
	{
		privEnqueueOutroActions();
	}

	if (privCheckShouldFightChange() && privIsNewFightAvailable())
	{
		privEnterNewFight();
	}
	else if (privCheckShouldFightChange())
	{
		myCurrentFight = 0;
	}

	if (privIsFighting())
	{
		privUpdateActions();
		privPlayerCharacterLogic();
		privEnemyAndOtherEntitiesLogic();
		privUpdateBattleMonitors();
	}

	if (privCheckShouldEnterPreGameOver())
	{
		privEnterPreGameOver();
	}
}

void Battle::privUpdateActions()
{
	// Pop the next action from the queue
	while (myCurrentAction == NULL && myActionQueue.size() > 0) {
		myCurrentAction = myActionQueue.front();
		myActionQueue.pop_front();

		if (myCurrentAction->getPerformer() != 0 
			&& !isOnBattlefield(myCurrentAction->getPerformer())) {
			myCurrentAction->cancel();
		}

		if (myCurrentAction->getPerformer() != 0 
			&& isOnBattlefield(myCurrentAction->getPerformer())
			&& myCurrentAction->getPerformer()->isDead()) {
				myCurrentAction->cancel();
		}

		if (!myCurrentAction->isCancelled()) {
			// Clean up targets that have dissapeared or died
			std::vector<Character*> targetsToRemove;
			for (int i = 0; i < myCurrentAction->getNumTargets(); i++)
			{
				Character *target = myCurrentAction->getTarget(i);

				if (!isOnBattlefield(target))
				{
					targetsToRemove.push_back(target);
					continue;
				}

				if (myCurrentAction->getActionTrigger().getTargetPolicy() == ActionTrigger::TargetPolicy_OnlyAlive && target->isDead())
				{
					targetsToRemove.push_back(target);
					continue;
				}
			}

			if (!myCurrentAction->hadSingleTarget()) {
				// If we had many targets then we just remove the invalid ones
				for (int i = 0; i < (int)targetsToRemove.size(); i++)
				{	
					myCurrentAction->removeTarget(targetsToRemove[i]);
				}
			} else if (targetsToRemove.size() > 0) {
				
				if (myCurrentAction->getTargetFaction() == Character::Faction_Enemy
					&& isAllEnemiesDead()) 
				{
					myCurrentAction->cancel();
				}

				if (myCurrentAction->getTargetFaction() == Character::Faction_Player
					&& privIsAllFiightingPlayersDead()) 
				{
					myCurrentAction->cancel();
				}

				// If we had a single target then we try to replace it with something else from the same faction
				Character *target = targetsToRemove[0];
				std::vector<Character *> replacementPool;

				if (myCurrentAction->getTargetFaction() == Character::Faction_Enemy) 
				{
					replacementPool = getEnemyCharacters();
				} 
				else if (myCurrentAction->getTargetFaction() == Character::Faction_Player) 
				{
					replacementPool = getFightingPlayerCharacters();
				}

				if (replacementPool.size() == 0) {
					myCurrentAction->cancel();
				} else {
					Character *replacement = replacementPool.at(rand() % replacementPool.size());
					std::vector<Character *> newTargets;
					newTargets.push_back(replacement);
					myCurrentAction->setTargets(newTargets);
				}
			}
		}

		if (myCurrentAction->isCancelled()) {
			delete myCurrentAction;
			myCurrentAction = NULL;
		} else {
			myCurrentAction->onEnter(this);
		}
	}

	// Poll and remove action.
	if (myCurrentAction != NULL) {
		myCurrentAction->onLogic();

		if (myCurrentAction->isDone()) {
			myCurrentAction->onLeave();
			if (myCurrentAction->getPerformer() != NULL) {
				myCurrentAction->getPerformer()->setQueuedAction(NULL); // TODO: Instead of null, check the queue?
			}
			delete myCurrentAction;
			myCurrentAction = NULL;

			if (myInCleanupPhase) {
				privCleanUpDeadEnemies();
				myInCleanupPhase = false;
			} else {
				myCurrentAction = new ActionRemoveDeadCreatures();
				myCurrentAction->onEnter(this);
				myInCleanupPhase = true;
			}
			
		}
	}
}


void Battle::privCleanUpDeadEnemies()
{
	std::vector<EnemyCharacter*>::iterator it;

	for (it = myEnemyCharacters.begin(); it != myEnemyCharacters.end(); )
	{
		if ((*it)->isToBeDestroyed())
		{
			delete *it;
			it = myEnemyCharacters.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void Battle::onDraw(BITMAP* buffer)
{
	std::vector<Entity *> allEntities;

	for (int i = 0; i < (int)myPlayerCharacters.size(); i++)
	{
		allEntities.push_back(myPlayerCharacters[i]);
	}

	for (int i = 0; i < (int)myEnemyCharacters.size(); i++)
	{
		allEntities.push_back(myEnemyCharacters[i]);
	}

	for (int i = 0; i < (int)myOtherEntities.size(); i++)
	{
		allEntities.push_back(myOtherEntities[i]);
	}

	struct { bool operator() (Entity *a, Entity *b) { return a->getZOrder() < b->getZOrder(); } } EntityComparator;
	sort(allEntities.begin(), allEntities.end(), EntityComparator);

	rectfill(buffer, 0, 0, 320, 250, makecol(50, 50, 50));

	for (int i = 0; i < (int)allEntities.size(); i++)
	{
		allEntities[i]->onRender(buffer);
	}
}



Character * Battle::findCharacter( const std::string name )
{
	for (int i = 0; i < (int)myPlayerCharacters.size(); i++) {
		if (myPlayerCharacters[i]->getName() == name) {
			return myPlayerCharacters[i];
		}
	}

	for (int i = 0; i < (int)myEnemyCharacters.size(); i++) {
		if (myEnemyCharacters[i]->getName() == name) {
			return myEnemyCharacters[i];
		}
	}

	return NULL;
}

const std::vector<PlayerCharacter *> &Battle::getAllPlayerCharacters()
{
	return myPlayerCharacters;
}

std::vector<Character *> Battle::getFightingPlayerCharacters()
{
	std::vector<Character *> result;
	for (int i = 0; i < (int)myPlayerCharacters.size(); i++) {
		if (myPlayerCharacters[i]->isFighting()) {
			result.push_back(myPlayerCharacters[i]);
		}
	}
	return result;
}

std::vector<Character *> Battle::getAliveFightingPlayerCharacters()
{
	std::vector<Character *> result;
	for (int i = 0; i < (int)myPlayerCharacters.size(); i++) {
		if (myPlayerCharacters[i]->isFighting() && myPlayerCharacters[i]->getHP() != 0)
		{
			result.push_back(myPlayerCharacters[i]);
		}
	}
	return result;
}

std::vector<Character *> Battle::getEnemyCharacters()
{
	std::vector<Character *> result;
	for (int i = 0; i < (int)myEnemyCharacters.size(); i++) {
		result.push_back(myEnemyCharacters[i]);
	}
	return result;
}

std::vector<Character *> Battle::getAliveEnemyCharacters()
{
	std::vector<Character *> result;
	for (int i = 0; i < (int)myEnemyCharacters.size(); i++) {
		result.push_back(myEnemyCharacters[i]);
	}
	return result;
}

void Battle::addEntity( Entity *entity )
{
	PlayerCharacter *playerCharacter = dynamic_cast<PlayerCharacter*>(entity);
	EnemyCharacter *enemyCharacter = dynamic_cast<EnemyCharacter*>(entity);

	if (playerCharacter != NULL) {
		myPlayerCharacters.push_back(playerCharacter);
		playerCharacter->setBattle(this);
	} else if (enemyCharacter != NULL) {
		myEnemyCharacters.push_back(enemyCharacter);
		enemyCharacter->setBattle(this);
	} else {
		myOtherEntities.push_back(entity);
	}
}

bool Battle::privIsPlayerCharacterInActionQueue( PlayerCharacter* aPlayerCharacter )
{
	std::deque<PlayerCharacter*>::iterator iter;
	for (iter = myPlayerCharacterActionQueue.begin(); iter != myPlayerCharacterActionQueue.end(); iter++)
	{
		if (*iter == aPlayerCharacter)
		{
			return true;
		}
	}

	return false;
}

void Battle::privPlayerCharacterLogic()
{
	for (int i = 0; i < (int)myPlayerCharacters.size(); i++)
	{
		PlayerCharacter* p = myPlayerCharacters[i];
		p->onLogic();
		if (p->isReadyForAction() && !privIsPlayerCharacterInActionQueue(p))
		{
			myPlayerCharacterActionQueue.push_back(p);
		}
	}

	while(myPlayerCharacterActionQueue.size() > 0)
	{	
		PlayerCharacter* p = myPlayerCharacterActionQueue.front();
		if (!p->isReadyForAction() || p->isDead())
		{
			myPlayerCharacterActionQueue.pop_front();
		}
		else
		{
			myPlayerCharacterWithActionChoice = p;
			break;
		}
	}


	if (myPlayerCharacterActionQueue.size() == 0)
	{
		myPlayerCharacterWithActionChoice = 0;
	}

	std::vector<PlayerCharacter *>::iterator it;
	for (it = myPlayerCharacters.begin(); it != myPlayerCharacters.end();)
	{
		if ((*it)->isToBeDestroyed())
		{
			it = myPlayerCharacters.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void Battle::placePlayerWithActionChoiceLastInQueue()
{
	myPlayerCharacterActionQueue.pop_front();
	myPlayerCharacterActionQueue.push_back(myPlayerCharacterWithActionChoice);
}

PlayerCharacter* Battle::getPlayerCharacterWithActionChoice() const
{
	return myPlayerCharacterWithActionChoice;
}

void Battle::enqueueAction( Action *action, bool topPriority /*= false*/ )
{
	if (action->getPerformer() != 0)
		action->getPerformer()->setQueuedAction(action);

	if (topPriority) {
		myActionQueue.push_front(action);
	} else {
		myActionQueue.push_back(action);
	}
}

bool Battle::isOnBattlefield( Character *character )
{
	std::vector<Character *> characters = getFightingPlayerCharacters();
	for (int i = 0; i < (int)characters.size(); i++)
	{
		if (characters[i] == character) {
			return true;
		}
	}

	characters = getEnemyCharacters();
	for (int i = 0; i < (int)characters.size(); i++)
	{
		if (characters[i] == character) {
			return true;
		}
	}

	return false;
}

Gui* Battle::getGui() const
{
	return myGui;
}

void Battle::enqueueFight( Fight* fight )
{
	myFights.push_back(fight);
}

bool Battle::privCheckShouldFightChange()
{
	if (myCurrentFight == 0)
	{
		return true;
	}

	if(isAllEnemiesDead() && myActionQueue.size() == 0 && myCurrentAction == 0) 
	{
		return true;
	}

	return false;
}

bool Battle::privCheckShouldEnterPreGameOver()
{
	if (myBattleState == State_PreGameOver || myBattleState == State_GameOver)
		return false;


	if (myFights.size() == 0 && myCurrentFight == 0)
	{
		return true;
	}

	bool allPlayersDead = true;
	for (std::vector<PlayerCharacter *>::iterator pcit = myPlayerCharacters.begin(); pcit != myPlayerCharacters.end(); pcit++) {
		if(!(*pcit)->isDead()) {
			allPlayersDead = false;
			break;
		}
	}
	if(allPlayersDead) {
		return true;
	}

	return false;
}

void Battle::privEnterNewFight()
{
	for (unsigned int i = 0; i < myOtherEntities.size(); i++)
	{
		delete myOtherEntities[i];
	}

	myOtherEntities.clear();
	myCurrentFight = myFights.front();
	myFights.pop_front();
	std::vector<Entity *> entities = myCurrentFight->getEntities();

	for(std::vector<Entity *>::iterator it = entities.begin(); it != entities.end(); it++) {
		addEntity(*it);
	}
	std::vector<Action *> introActions = myCurrentFight->getIntroActions();
	for(std::vector<Action *>::iterator it = introActions.begin(); it != introActions.end(); it++) {
		enqueueAction(*it);
	}
	if (myCurrentFight->getMusic() != "")
		Music::playSong(myCurrentFight->getMusic());

	myOutroActionsEnqueued = false;
	
	for (unsigned int i = 0; i< myBattleMonitors.size(); i++)
	{
		delete myBattleMonitors[i];
	}

	myBattleMonitors.clear();

	for (unsigned int i = 0; i< myCurrentFight->getBattleMonitors().size(); i++)
	{
		BattleMonitor* bm = myCurrentFight->getBattleMonitors()[i];
		myBattleMonitors.push_back(bm);
		bm->setBattle(this);
	}
}

void Battle::privEnemyAndOtherEntitiesLogic()
{
	for (int i = 0; i < (int)myEnemyCharacters.size(); i++)
	{
		if (!myEnemyCharacters[i]->isToBeDestroyed()) {
			myEnemyCharacters[i]->onLogic();
		}
	}

	std::vector<Entity *>::iterator it;
	for (it = myOtherEntities.begin(); it != myOtherEntities.end(); )
	{
		if ((*it)->isToBeDestroyed())
		{
			delete *it;
			it = myOtherEntities.erase(it);
		}
		else
		{
			(*it)->onLogic();
			++it;
		}
	}
}

void Battle::privPreGameOverLogic()
{
	if (myBattleState != State_PreGameOver)
		return;

	myPreGameOverFrameCounter++;
	if(myPreGameOverFrameCounter > 100) {
		if(!myGameOverScreen) {
			myGameOverScreen = new GameOverScreen();
			myBattleState = State_GameOver;
			ScreenManager::add(myGameOverScreen);
		}
	}
}

void Battle::privEnterPreGameOver()
{
	myBattleState = State_PreGameOver;
	myPreGameOverFrameCounter = 0;
}

bool Battle::isAllEnemiesDead()
{
	for(unsigned int i = 0; i < myEnemyCharacters.size(); i++) {
		if(!myEnemyCharacters[i]->isDead()) {
			return false;
		}
	}

	return true;
}

bool Battle::privCheckShouldEnqueueFightOutroActions()
{
	return isAllEnemiesDead() && !myOutroActionsEnqueued && myActionQueue.size() == 0;	
}

bool Battle::privIsNewFightAvailable()
{
	return myFights.size() != 0;
}

bool Battle::privIsFighting()
{
	return myCurrentFight != 0;
}

void Battle::privEnqueueOutroActions()
{
	std::vector<Action *> actions = myCurrentFight->getOutroActions();
	for(std::vector<Action *>::iterator it = actions.begin(); it != actions.end(); it++) {
		enqueueAction(*it);
	}
	myOutroActionsEnqueued = true;
}

void Battle::privUpdateBattleMonitors()
{
	for (unsigned int i = 0; i < myBattleMonitors.size(); i++)
	{
		if (!myBattleMonitors[i]->isDone())
			myBattleMonitors[i]->onLogic();
	}
}

bool Battle::privIsAllFiightingPlayersDead()
{
	std::vector<Character*> players = getFightingPlayerCharacters();
	for (unsigned int i = 0; i < players.size(); i++)
	{
		Character* p = players[i];
		if (!p->isDead())
		{
			return false;
		}
	}

	return true;
}
