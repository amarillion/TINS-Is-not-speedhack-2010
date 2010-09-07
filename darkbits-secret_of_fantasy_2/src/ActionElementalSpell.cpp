#include "Precompiled.hpp"
#include "ActionElementalSpell.hpp"
#include "Character.hpp"
#include "Battle.hpp"
#include "ParticleEntity.hpp"
#include "Gui.hpp"

ActionElementalSpell::ActionElementalSpell(const std::string& name, Character::Damage damageType, int spellLevel)
	: ActionCast(name)
	, myDamageType( damageType )
	, mySpellLevel( spellLevel )
	, myCastTime( 0)
{
	
}

void ActionElementalSpell::onPrepareCast()
{
	getPerformer()->adjustMP(-getActionTrigger().getMPCost());
	myCastTime = 0;
}

void ActionElementalSpell::onCastLogic()
{
	if (myCastTime > 40)
		return;

	myCastTime++;

	for (int i = 0; i < getNumTargets(); i++) {
		Character *target = getTarget(i);
		ParticleEntity *particle = new ParticleEntity("data/images/testparticle.bmp", 1, 0.1, target->getCenterX() + rand() % 16, target->getCenterY() + rand() % 16, target->getZOrder() + 0.1f * myCastTime);
		particle->setGravity(0.0, -500.0);
		getBattle()->addEntity(particle);
	}

	if (myCastTime > 40)
	{
		for (int i = 0; i < getNumTargets(); i++) {
			getTarget(i)->damage(mySpellLevel * 10, myDamageType);
		}
	}
}

bool ActionElementalSpell::isCastDone()
{
	return myCastTime > 40;
}
