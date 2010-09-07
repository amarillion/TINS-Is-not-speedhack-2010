#pragma once

class Entity;
class Action;
class BattleMonitor;

class Fight
{
public:
	Fight();
	~Fight();

	const std::vector<Entity *> &getEntities();
	const std::vector<Action *> &getIntroActions();
	const std::vector<Action *> &getOutroActions();
	const std::vector<BattleMonitor*> &getBattleMonitors();

	const std::string getMusic();

	void addEntity(Entity* entity);
	void addIntroAction(Action* a);
	void addOutroAction(Action* a);
	void addBattleMonitor(BattleMonitor* b);
	void setMusic(const std::string &aMusic);


private:
	std::vector<Entity *> myEntities;
	std::vector<Action *> myIntroActions;
	std::vector<Action *> myOutroActions;
	std::vector<BattleMonitor *> myBattleMonitors;
	std::string myMusic;
};