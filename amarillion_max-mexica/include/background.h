#include "layer.h"

#ifndef BACKGROUND_H
#define BACKGROUND_H

class Background : public Layer
{
private:
	BITMAP *texture;
	
	int xofst;
	int yofst;
	
public:
	Background(Engine &e);
	virtual ~Background();
	
	void init();
	void update();
	void draw(BITMAP *buffer);
};

#endif
