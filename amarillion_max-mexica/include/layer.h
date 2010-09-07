#include <allegro.h>

#ifndef LAYER_H
#define LAYER_H

class Engine;

/**
	ViewPort provides a way for layers to move together
*/
class ViewPort
{
	private:
		int xofst;
		int yofst;
		
		// base offset without tremble
		int basex;
		int basey;
		
		// destination for movement
		int destx;
		int desty;
		
		float phase;
		float ampl;
	public: 
		ViewPort () : xofst (0), yofst (0), basex(0), basey(0), destx (0), 
			desty (0), phase(0), ampl(0) {}
		int getXofst() { return xofst; }
		int getYofst() { return yofst; }
		
		// set directly
		void setOfst (int _xofst, int _yofst) { xofst = _xofst; yofst = _yofst; basex = xofst; basey = yofst; destx = xofst; desty = yofst; }
		void moveTo (int _xofst, int _yofst);
		void update();
		void tremble (float ampl);
};

class Layer
{
	public:
		enum LayerState { LAYER_SLEEP, LAYER_FOREGROUND, LAYER_BACKGROUND };
	private:
		LayerState state;
		int zorder;
	protected:
		/**
			ViewPort is optional
		*/
		Engine &parent;
		ViewPort *viewPort;
	public:
		ViewPort *getViewPort() { return viewPort; }
		Engine &getParent() { return parent; }
		Layer (Engine &engine);
		
		void setViewPort (ViewPort *view) { viewPort = view; }
		virtual void foreground();
		virtual void background();
		virtual void stop();
		
		/**
			Update for layer with focus, only called if it has LAYER_FOREGROUND state
		*/
		virtual void handleInput();
		
		/**
			Update layer animation etc. called if layer has LAYER_FOREGROUND or LAYER_BACKGROUND state
		*/
		virtual void update();
		
		virtual void draw(BITMAP *dest);
		LayerState getState () { return state; }
		int getZOrder () { return zorder; }
};

#endif
