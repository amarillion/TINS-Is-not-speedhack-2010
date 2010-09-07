#include <assert.h>
#include "engine.h"
#include "color.h"
#include <math.h>
#include <list>
#include <algorithm>
#include "game.h"
#include "engine.h"
#include "color.h"
#include "anim.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "player.h"
#include "main.h"

using namespace std;

class MyObjectRemover
{
public:
	bool operator()(Sprite *s)
	{
		if (!s->isAlive())
		{
			delete s;
			return true;
		}
		else
			return false;
	}
};

void Game::generatedObjects()
{
	if (enemyCount < currentLevelData.pepperNum)
	{
		if (enemyTimer > 0)
			enemyTimer -= Main::logicIntervalMsec;
		else
		{
			addEnemy();
			enemyTimer = currentLevelData.pepperMsecMin + rand() % currentLevelData.pepperMsecDev;
		}
	}

	if (foodCount < 1)
	{
		if (foodTimer > 0)
			foodTimer -= Main::logicIntervalMsec;
		else
		{
			int x = rand() % SCREEN_W;
			SpriteEx *bonus = new SpriteEx(this, ST_FOOD, x, 0, 0, 0, rand() % 3);
			add (bonus);

			foodTimer = currentLevelData.foodMsecMin + rand() % currentLevelData.foodMsecDev;
			foodCount++;
		}
	}

	if (heartCount < 1)
	{
		if (heartTimer > 0)
			heartTimer -= Main::logicIntervalMsec;
		else
		{
			int x = rand() % SCREEN_W;
			SpriteEx *bonus = new SpriteEx(this, ST_HEART, x, 0);
			add (bonus);

			heartTimer = 3000 + rand() % 3000;
			heartCount++;
		}
	}
}

void Game::update ()
{
	if (!replay->update()) { done(); return; }

	// GS_NEXT_LEVEL is a special transition state...
	if (state == GS_NEXT_LEVEL)
	{
		killAll();
		initLevel();
		state = GS_LEVEL_INTRO;
		stateTimer = 0;
	}

	stateTimer++;
	switch (state)
	{
	case GS_GAMEOVER:
		if (stateTimer > 100)
		{
			done();
		}
		break;
	case GS_LEVEL_INTRO:
		if (stateTimer > 100)
		{
			state = GS_PLAYING;
			stateTimer = 0;
		}
	case GS_PAUSE:
		if (stateTimer > 100)
		{
			if (key[KEY_ESC])
			{
				done();
			}
			else if (key[KEY_SPACE])
			{
				state = GS_PLAYING;
			}
		}
		break;
	case GS_PLAYING:
		updateObjects();
		if (key[KEY_ESC])
		{
			state = GS_PAUSE;
			break;
		}
		break;
	}
}

void Game::updateObjects()
{
	generatedObjects();

	//update
	list<Sprite*>::iterator i;
	for (i = sprites.begin(); i != sprites.end(); i++)
	{
		if ((*i)->isAlive()) (*i)->update();
	}
	
	// detect collisions
	for (i = sprites.begin(); i != sprites.end(); i++)
	{
		list<Sprite*>::iterator j;
		for (j = sprites.begin(); j != sprites.end(); j++)
		{
			if (*i == *j) continue;
			if (!(*i)->isAlive()) continue;
			if (!(*j)->isAlive()) continue;

			if ((*i)->hasOverlap(*j))
			{
				(*i)->onCol (*j);
			}

		}
	}

	// remove all that are not alive!
	sprites.remove_if (MyObjectRemover());
}

void Game::done()
{
	killAll();
	set_config_int ("tins10", "hiscore", hi);
	parent.setEvent(Engine::E_FINISHED);
}

Game::Game (Engine &engine) : Layer (engine), sprites(), replay(NULL)
{
	hi = 0;
}

void Game::draw (BITMAP *buffer)
{

	switch (state)
	{
	case GS_GAMEOVER:
		clear_to_color(buffer, WHITE);
		alfont_textprintf_centre_aa_ex (buffer, gamefont, SCREEN_W/2, SCREEN_H/2, BLACK, -1, "GAMEOVER");
		break;
	case GS_PLAYING:
		draw_sprite(buffer, bgImage, 0, 0);
		alfont_textprintf_aa_ex (buffer, gamefont, 0, 8, BLACK, -1, "SCORE: %06i  HI: %06i  LIVES: %02i  HP: %02i  BLOOD: %02i", score, hi, lives, hp, dropped, enemyTimer);
		{
			list<Sprite*>::iterator i;
			for (i = sprites.begin(); i != sprites.end(); i++)
			{
				if ((*i)->isVisible()) (*i)->draw(buffer, 0, 0);
			}
		}
#ifdef DEBUG
		if (parent.isDebug())
		{
			textprintf_ex (buffer, font, 0, 16, GREEN, -1, "%d sprites", sprites.size());
		}
#endif
		break;
	case GS_NEXT_LEVEL:
	case GS_LEVEL_INTRO:
		clear_to_color(buffer, WHITE);
		alfont_textprintf_centre_aa_ex (buffer, gamefont, SCREEN_W/2, SCREEN_H/2 - 40, BLACK, -1, "LEVEL %02i", currentLevel+1);
		alfont_textprintf_centre_aa_ex (buffer, gamefont, SCREEN_W/2, SCREEN_H/2 + 40, BLACK, -1, "PLAYER x %02i", lives);
		break;
	case GS_PAUSE:
		clear_to_color(buffer, WHITE);
		alfont_textprintf_centre_aa_ex (buffer, gamefont, SCREEN_W/2, SCREEN_H-16, BLACK, -1, "Press ESC again to stop, or SPACE to play");
		break;
	}
}

map<string, Anim*> Game::anims;

void Game::init()
{
	gamefont = parent.getResources()->getAlfont("Olivia1_24");
	smallfont = parent.getResources()->getAlfont("Olivia1_16");
	anims = parent.getResources()->getAnimList();

	RLE_SPRITE *rle1 = parent.getResources()->getRle("background");
	BITMAP *temp = create_bitmap(rle1->w, rle1->h);
	draw_rle_sprite (temp, rle1, 0, 0);

	bgImage = create_bitmap (SCREEN_W, SCREEN_H);
	stretch_sprite (bgImage, temp, 0, 0, SCREEN_W, SCREEN_H);
	destroy_bitmap(temp);

	hi = get_config_int ("tins10", "hiscore", 0);
	//TODO: also destroy bgImage at shutdown time

}

void Game::initGame(IReplay *_replay)
{
	replay = _replay;
	currentLevel = 0;
	lives = START_LIVES;
	score = 0;
	hp = START_HP;
	state = GS_LEVEL_INTRO;
	stateTimer = 0;
	initLevel();
}

void Game::initLevel()
{
	parent.playSample("OhHai");
	currentLevelData = parent.getResources()->getLevelData(currentLevel);

	add (new SpriteEx (this, ST_FOLE, 480, 40));

	player = new Player(this, SCREEN_W/2, 64);
	add (player);

	SpriteEx *platform;
	platform = new SpriteEx (this, ST_PLATFORM, 128, 192,   160, 32);
	add (platform);

	platform = new SpriteEx (this, ST_PLATFORM, 384, 384,   128, 32);
	add (platform);

	platform = new SpriteEx (this, ST_PLATFORM, 736, 288,   192, 32);
	add (platform);

	dropzone = new SpriteEx (this, ST_DROPZONE, 608, 576,    80, 64);
	add (dropzone);


	dropped = 0;

	heartTimer = 0;
	foodTimer = 0;
	enemyTimer = 0;
	heartCount = 0;
	enemyCount = 0;
	foodCount = 0;
}

void Game::dropHeart()
{
	dropped++;
	if (dropped == DROPS_NEEDED)
	{
		currentLevel++;
		state = GS_NEXT_LEVEL;
		stateTimer = 0;
	}
}

void Game::playerHit()
{
	hp--;
	if (hp == 0)
	{
		lives--;
		hp = START_HP;

		if (lives > 0)
		{
			state = GS_LEVEL_INTRO;
			stateTimer = 0;
		}
		else
		{
			state = GS_GAMEOVER;
			parent.playSample("gameover");
			stateTimer = 0;
		}
	}
}

void Game::addEnemy()
{
	int x = SCREEN_W;
	int y = rand() % 400;
	Enemy *enemy = new Enemy(this, x, y, currentLevelData.pepperType);
	add (enemy);
	enemyCount++;
}

void Game::killAll()
{
	list<Sprite*>::iterator i;
	for (i = sprites.begin(); i != sprites.end(); ++i)
	{
		delete (*i);
		(*i) = NULL;
	}
	sprites.clear();
}

void Game::add(Sprite *o)
{
	sprites.push_back (o);
}

void Game::addScore (int amount)
{
	score += amount;
	if (score > hi) hi = score;
}

Sprite::Sprite (Game *game, int amx, int amy) : w(0), h(0), anim(NULL), state(0), dir(0), animStart(0)
{
	x = amx;
	y = amy;
	alive = true;
	visible = true;
	parent = game;
}

void Sprite::update()
{
	assert (parent);
}

void Sprite::kill()
{
	alive = false;
}

void Sprite::draw(BITMAP * buffer, int xofst, int yofst)
{
	int x = getx() + xofst;
	int y = gety() + yofst;
	int time = 0;
	if (anim != NULL)
		anim->drawFrame (buffer, state, dir, time, x, y);
}

bool Sprite::hasXOverlap (Sprite *s2)
{
	int x1 = x;
	int dx1 = w;
	int x2 = s2->getx();
	int dx2 = s2->getw();
	return !(
			(x1 >= x2+dx2) || (x2 >= x1+dx1)
		);
}

bool Sprite::hasOverlap (Sprite *s2)
{
	int y1 = y;
	int dy1 = h;
	int y2 = s2->gety();
	int dy2 = s2->geth();
	return hasXOverlap(s2) &&
		!(
			(y1 >= y2+dy2) || (y2 >= y1+dy1)
		);
}
