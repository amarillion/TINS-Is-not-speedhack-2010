#include "Precompiled.hpp"
#include "Time.hpp"
#include "ParticleEntity.hpp"


ParticleEntity::ParticleEntity( const std::string& animationFilename, int numFrames, float speed, float x, float y, float zOrder )
	: myAnimation( animationFilename, numFrames )
	, mySpeed( speed )
	, myX( x ), myY( y )
	, myZOrder( zOrder )
	, myVelX( 0.0f ), myVelY( 0.0f ) 
	, myGravityX( 0.0f ), myGravityY( 0.0f )
	, myDamping( 1.0f )
	, myBlending( Blending_None )
	, myLoops( 1 )
	, myFrame( 0 )
	, myTime( 0 )
{

}

ParticleEntity * ParticleEntity::setVelocity( float velX, float velY )
{
	myVelX = velX;
	myVelY = velY;
	return this;
}

ParticleEntity * ParticleEntity::setGravity( float gravityX, float gravityY )
{
	myGravityX = gravityX;
	myGravityY = gravityY;
	return this;
}

ParticleEntity * ParticleEntity::setDamping( float damping )
{
	myDamping = damping;
	return this;
}

ParticleEntity * ParticleEntity::setBlending( Blending blending )
{
	myBlending = blending;
	return this;
}

ParticleEntity * ParticleEntity::setLoops( int loops )
{
	myLoops = loops;
	return this;
}

void ParticleEntity::onLogic()
{
	float dt = 1.0f / Time::TicksPerSecond;
	myFrame = myTime * mySpeed;

	myVelX += myGravityX * dt;
	myVelY += myGravityY * dt;
	myVelX *= myDamping;
	myVelY *= myDamping;
	myX += myVelX * dt;
	myY += myVelY * dt;

	myTime++;
}

void ParticleEntity::onRender( BITMAP *dest )
{
	if (isToBeDestroyed()) {
		return;
	}

	int x = (int)(myX - myAnimation.getFrameWidth() * 0.5);
	int y = (int)(myY - myAnimation.getFrameHeight() * 0.5);
	myAnimation.drawFrame(dest, myFrame, x, y, false, false, myBlending);
}

float ParticleEntity::getZOrder()
{
	return myZOrder;
}

bool ParticleEntity::isToBeDestroyed()
{
	return myFrame > myAnimation.getFrameCount() * myLoops;
}

int ParticleEntity::getX()
{
	return myX;
}

int ParticleEntity::getY()
{
	return myY;
}