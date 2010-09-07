#pragma once

class Entity {
protected:
	Entity() {}

public:
	virtual ~Entity() {};

	virtual void onLogic() = 0;

	virtual void onRender(BITMAP *dest) = 0;

	virtual float getZOrder() = 0;

	virtual bool isToBeDestroyed() = 0;

	virtual int getX() = 0;

	virtual int getY() = 0;
};