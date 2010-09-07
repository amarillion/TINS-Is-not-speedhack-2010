#include "Entity.hpp"

class Font;

class HackEntity : public Entity {
public:
	HackEntity(int aTimeToShow);

	virtual void onLogic();

	virtual void onRender(BITMAP *dest);

	virtual float getZOrder();

	virtual bool isToBeDestroyed();

	virtual int getX();

	virtual int getY();

private:
	int myX;
	int myY;
	std::vector<std::string> myLinesOfCode;
	int myFrameCounter;
	Font* myFont;
	int myTimeToShow;
};