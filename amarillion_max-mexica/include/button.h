#ifndef BUTTON_H
#define BUTTON_H

class Button
{
private:
	int scancode; // contains keyboard scancode
	int altcode;
	
	bool prevState;
	
public:
	Button (int _scancode = 0, int _altcode = 0) 
	{ scancode = _scancode; altcode = _altcode; prevState = false;}
	bool getState();
	
	/**
		returns true if 
		the state of this button was false on last call and true on this call
	*/
	bool justPressed();

	void setScancode (int newcode) { scancode = newcode; }
	void setAltcode (int newcode) { altcode = newcode; }
	int getScancode () { return scancode; }
	const char *name();
};

// install the keyboard lowlevel callback
void initKeyboard ();

// reset scancode to 0 so you can start monitoring for the next keypress
void resetLastScancode();

// return last pressed key. Should return 0 after resetLastScancode
int getLastScancode();

#endif
