#include "Entity.hpp"

class Font;

class BouncyTextEntity : public Entity {
public:
	BouncyTextEntity(const std::string &text, int x, int y, unsigned int color, int zOffs = 0);
	virtual void onLogic();

	virtual void onRender(BITMAP *dest);

	virtual float getZOrder();

	virtual bool isToBeDestroyed();

	virtual int getX();
	
	virtual int getY();

private:
	struct BouncyCharacter {
		int x;
		int y;
		char c;
			
	}; 
	Font *myFont;
	std::string myText;
	int myX, myY;
	int myFrameCounter;
	int myZOffset;
	
};