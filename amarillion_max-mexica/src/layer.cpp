#include "layer.h"
#include "engine.h"
#include "color.h"
#include <math.h>

void Layer::background()
{
	state = LAYER_BACKGROUND;
}

void Layer::foreground()
{
	state = LAYER_FOREGROUND;
}

void Layer::stop()
{
	state = LAYER_SLEEP;;
}

void Layer::handleInput()
{
}

void Layer::update()
{
}

void Layer::draw(BITMAP * dest)
{
	// default implementation: clear blue
	clear_to_color (dest, BLUE);
}

Layer::Layer(Engine & _engine) : state (LAYER_SLEEP), parent (_engine), viewPort (NULL) 
{
	zorder = parent.getMaxZOrder();
}

void ViewPort::update()
{
	int dx = destx - basex;
	int dy = desty - basey;
	
	// if close by, move directly
	if ((dx * dx + dy * dy) < 20.0)
	{
		basex = destx;
		basey = desty;
	}
	else
	{
		// move there in increasingly smaller steps
		basex += dx / 12;
		basey += dy / 12;
	}

	if (ampl > 0)
	{
		phase += 1.5;
		float xval = ampl * sin(phase * 1.3);
		float yval = ampl * sin(phase * 0.9);
		ampl -= 0.2; // decay
		
		xofst = basex + (int)xval;
		yofst = basey + (int)yval;
	}
	else
	{
		xofst = basex;
		yofst = basey;
	}
}

void ViewPort::moveTo(int _xofst, int _yofst)
{
	destx = _xofst;
	desty = _yofst;
}

void ViewPort::tremble(float _ampl)
{
	ampl = _ampl;
	phase = 0;
}
