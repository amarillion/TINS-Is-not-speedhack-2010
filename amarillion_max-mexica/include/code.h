#ifndef CODE_H
#define CODE_H

#include <vector>
#include <alfont.h>
#include <string>

#include "layer.h"

class Resources;

class Code : public Layer
{
private:
	ALFONT_FONT *codefont;

	const char *currentFile;
	std::vector<std::string> lines;
	std::vector<std::string> srcFiles;
	int filePos;
	int ofst;

	void loadFile(std::string &filename);
	void drawCode (BITMAP *dest, int x, int y, int w, int h, int xofst, int yofst);

public:
	Code(Engine &e);
	void init();

	virtual void update();
	virtual void draw(BITMAP *dest);


};

#endif
