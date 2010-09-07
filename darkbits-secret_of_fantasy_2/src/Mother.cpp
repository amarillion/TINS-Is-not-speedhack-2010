#include "Precompiled.hpp"
#include "Mother.hpp"

Mother::Mother(int homeX, int homeY )
:PlayerCharacter("MOTHER",homeX, homeY, 1, 1, 400, 3.0f)
, myRemoveMyself(false)
{

}

void Mother::remove()
{
	myRemoveMyself = true;
}

bool Mother::isToBeDestroyed()
{
	if (myRemoveMyself)
		return true;
	return PlayerCharacter::isToBeDestroyed();
}
