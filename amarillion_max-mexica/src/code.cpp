#include "code.h"
#include "resources.h"
#include <iostream>
#include <fstream>
#include "color.h"
#include "engine.h"
#include "resources.h"

using namespace std;

void Code::loadFile(string &filename)
{
	lines.clear();
	ifstream f;
	string line;
	f.open (filename.c_str(), ios::in);
	if (f.is_open())
	{
		while (! f.eof())
		{
			getline (f, line);
			// replace tabs with spaces
			for (string::iterator i = line.begin(); i != line.end(); ++i)
			{
				if ((*i) == '\t')
					line.replace (i, i+1, " ");
			}

			lines.push_back(line);
		}
		f.close();
	}

	ofst = 0;
}

void Code::drawCode (BITMAP *dest, int x, int y, int w, int h, int xofst, int yofst)
{
	int lineh = alfont_get_font_height(codefont);

	int linepos = yofst / lineh;
	int ypos = y - (yofst % lineh);
	int max = lines.size();

	for (int i = 0; i < (h / lineh); ++i)
	{
		if (linepos > max) break;
		alfont_textout_aa_ex(dest, codefont, lines[linepos].c_str(), x - xofst, ypos, GREEN, -1);
		linepos++;
		ypos += lineh;
	}

	alfont_textout_right_aa_ex(dest, codefont, srcFiles[filePos].c_str(), SCREEN_W, 0, WHITE, -1);
}

void Code::init()
{
	codefont = parent.getResources()->getAlfont("Olivia1_16");

	glob("src/*.cpp", srcFiles);
	glob("include/*.h", srcFiles);
	filePos = 0;
	loadFile(srcFiles[filePos]);
}

Code::Code(Engine &e) : Layer(e), codefont(NULL), currentFile(NULL), lines(), filePos(0) {}

void Code::update()
{
	if (keypressed())
	{
		int val = readkey();
		switch (val >> 8)
		{
		case KEY_ESC:
			parent.setEvent(Engine::E_MAINMENU);
			break;
		case KEY_PGUP:
			ofst -= (SCREEN_H - 60);
			break;
		case KEY_PGDN:
			ofst += (SCREEN_H - 60);
			break;
		case KEY_DOWN:
			ofst += 20;
			break;
		case KEY_UP:
			ofst -= 20;
			break;
		case KEY_LEFT:
			filePos = (filePos + 1) % srcFiles.size();
			loadFile(srcFiles[filePos]);
			break;
		case KEY_RIGHT:
			filePos = (filePos - 1 + srcFiles.size()) % srcFiles.size();
			loadFile(srcFiles[filePos]);
			break;
		}
		if (ofst < 0) ofst = 0;
	}

}

void Code::draw(BITMAP *dest)
{
	clear_to_color (dest, BLACK);
	drawCode (dest, 0, 0, SCREEN_W, SCREEN_H, 0, ofst);
}
