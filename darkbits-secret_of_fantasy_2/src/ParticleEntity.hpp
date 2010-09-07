#pragma once

#include "Entity.hpp"
#include "Animation.hpp"

class ParticleEntity : public Entity {
public:
	ParticleEntity(const std::string& animationFilename, int numFrames, float speed, float x, float y, float zOrder);

	ParticleEntity *setVelocity(float velX, float velY);

	ParticleEntity *setGravity(float gravityX, float gravityY);

	ParticleEntity *setDamping(float damping);

	ParticleEntity *setBlending(Blending blending);

	ParticleEntity *setLoops(int loops);


	virtual void onLogic();

	virtual void onRender(BITMAP *dest);

	virtual float getZOrder();

	virtual bool isToBeDestroyed();

	virtual int getX();

	virtual int getY();

private:
	Animation myAnimation;
	float mySpeed;
	float myX, myY;
	float myZOrder;
	float myVelX, myVelY;
	float myGravityX, myGravityY;
	float myDamping;
	Blending myBlending;
	int myLoops;
	int myFrame;
	int myTime;
};