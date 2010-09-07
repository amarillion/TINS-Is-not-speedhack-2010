#include "Precompiled.hpp"
#include "ActionTrigger.hpp"


ActionTrigger::ActionTrigger()
	: myType(Type_Ability)
	, myTargetMode(TargetMode_JustOne)
	, myAlignment(Alignment_Offensive)
	, myTargetPolicy(TargetPolicy_DeadAllowed)
	, myMPCost( 0 )
	, myOrder( -1 )
{
}

ActionTrigger::ActionTrigger( Type type, const std::string &name, const std::string &description, TargetMode targetMode, TargetPolicy targetPolicy, Alignment alignment, int mpCost )
	: myType( type )
	, myName( name )
	, myDescription( description )
	, myTargetMode( targetMode )
	, myTargetPolicy( targetPolicy )
	, myAlignment( alignment )
	, myMPCost( mpCost )
	, myOrder( -1 )
{

}

ActionTrigger::Type ActionTrigger::getType() const
{
	return myType;
}

const std::string &ActionTrigger::getName() const
{
	return myName;
}

int ActionTrigger::getMPCost() const
{
	return myMPCost;
}

ActionTrigger::TargetMode ActionTrigger::getTargetMode() const
{
	return myTargetMode;
}

ActionTrigger::Alignment ActionTrigger::getAlignment() const
{
	return myAlignment;
}

int ActionTrigger::getOrder() const
{
	return myOrder;
}

void ActionTrigger::setOrder( int order )
{
	myOrder = order;
}

bool ActionTrigger::operator<( const ActionTrigger& that ) const
{
	return getOrder() < that.getOrder();
}

ActionTrigger::TargetPolicy ActionTrigger::getTargetPolicy() const
{
	return myTargetPolicy;
}

const std::string & ActionTrigger::getDescription() const
{
	return myDescription;
}
