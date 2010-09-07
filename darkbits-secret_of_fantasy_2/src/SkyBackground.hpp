#include "BackgroundEntity.hpp"
#include "Animation.hpp"

class SkyBackground : public BackgroundEntity {
public:
	SkyBackground();
	~SkyBackground();

	virtual void onLogic();

	virtual void onRender(BITMAP *dest);

private:
	struct Bird {
		Bird(int x, int y, float speed) : myX(x), myY(y), myFrameCounter(0), myRealX(x), mySpeed(speed) {
			myAnimation = new Animation("data/images/bird_animation.bmp", 2);
		};
		void onRender(BITMAP *dest) {
			myAnimation->drawFrame(dest, (myFrameCounter/3) * mySpeed, myX, myY);
		};
		void onLogic(){ 
			myFrameCounter ++;
			myRealX += mySpeed;
			myX = (int)myRealX;
		};
		Animation *myAnimation;
		int myX;
		int myY;
		unsigned int myFrameCounter;
		float mySpeed;
		float myRealX;
		
		
	};


	BITMAP *myBackgroundImage;
	std::vector<Bird *> myBirds;
};