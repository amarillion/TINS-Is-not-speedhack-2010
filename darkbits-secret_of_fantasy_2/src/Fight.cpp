#include "Precompiled.hpp"
#include "Fight.hpp"

Fight::Fight()
: myMusic("")
{
	
}

Fight::~Fight()
{

}

const std::vector<Entity *> & Fight::getEntities()
{
	return myEntities;
}

const std::vector<Action *> & Fight::getIntroActions()
{
	return myIntroActions;
}

const std::vector<Action *> & Fight::getOutroActions()
{
	return myOutroActions;
}

const std::string Fight::getMusic()
{
	return myMusic;
}

void Fight::addEntity( Entity* entity )
{
	myEntities.push_back(entity);
}

void Fight::addIntroAction( Action* a )
{
	myIntroActions.push_back(a);
}

void Fight::addOutroAction( Action* a )
{
	myOutroActions.push_back(a);
}

void Fight::addBattleMonitor( BattleMonitor* b )
{
	myBattleMonitors.push_back(b);
}

const std::vector<BattleMonitor*> & Fight::getBattleMonitors()
{
	return myBattleMonitors;
}

void Fight::setMusic( const std::string &aMusic )
{
	myMusic = aMusic;
}
