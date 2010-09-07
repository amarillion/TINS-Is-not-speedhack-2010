#include "Precompiled.hpp"
#include "BattleMonitor.hpp"
#include "Battle.hpp"

BattleMonitor::BattleMonitor(bool aMonitor(Battle *aBattle))
: myMonitor(aMonitor)
, myBattle(0)
,myIsDone(false)
{

}

Battle* BattleMonitor::getBattle()
{
	return myBattle;
}

void BattleMonitor::setBattle( Battle* battle )
{
	myBattle = battle;
}

void BattleMonitor::onLogic()
{
	myIsDone = myMonitor(myBattle);
}

bool BattleMonitor::isDone()
{
	return myIsDone;
}
