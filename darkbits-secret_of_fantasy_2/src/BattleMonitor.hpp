#pragma once

class Battle;

class BattleMonitor
{
public:
	BattleMonitor(bool aMonitor(Battle *aBattle));
	
	bool isDone();
	void onLogic();
	
	Battle* getBattle();
	void setBattle(Battle* battle);

private:
	Battle* myBattle;
	bool(*myMonitor)(Battle *aBattle);
	bool myIsDone;
};