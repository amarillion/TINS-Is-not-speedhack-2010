#ifndef PLAYER_H
#define PLAYER_H

#include "game.h"
#include "button.h"

enum SpriteType
{
	ST_PLAYER, ST_HEART, ST_ENEMY, ST_CEIL, ST_PLATFORM, ST_FOOD, ST_FLOOR, ST_DROPZONE, ST_HEART2, ST_FOLE, ST_SCORE
};

class SpriteEx : public Sprite
{
protected:
	bool air;
	bool gravity;
	float dy;
	float ddy;
	float dx;
	SpriteType spriteType;
	int subtype;
public:
	SpriteEx(Game *, SpriteType st, int x, int y, int _w = DEFAULT_SPRITE_W, int _h = DEFAULT_SPRITE_H, int subtype = 0);
	virtual void draw(BITMAP *buffer, int xofst, int yofst);
	virtual void update();
	virtual void onCol (Sprite *s);
	virtual void onVBounce(SpriteType st, SpriteEx *s2);
	virtual void onHBounce(SpriteType st, SpriteEx *s2);
	SpriteType getSpriteType() { return spriteType; }
};

class ScoreText : public SpriteEx
{
private:

	int timer;
	BITMAP *cache;
public:
	ScoreText(Game * game, int score, int x, int y);
	~ScoreText();

	virtual void draw(BITMAP *buffer, int xofst, int yofst);
	virtual void update() { timer--; if (timer < 0) kill(); SpriteEx::update(); }
};

class Player : public SpriteEx
{
private:
	IReplay *replay;
	SpriteEx *platform;
public:
	bool loaded;
	int hitTimer;
	Player(Game *, int x, int y);
	virtual void update();
	virtual void onCol (Sprite *s);
	virtual void onVBounce(SpriteType st, SpriteEx *s2);
	virtual void onHBounce(SpriteType st, SpriteEx *s2);
};

class Enemy : public SpriteEx
{
	int enemyType;
	bool floorKills;
	bool hbounceKills;
	int phase;
	int period;
	float hsign;
	float vsign;
public:
	Enemy(Game *, int x, int y, int _type);
	virtual void update();
	virtual void onVBounce(SpriteType st, SpriteEx *s2);
	virtual void onHBounce(SpriteType st, SpriteEx *s2);
	virtual void kill();
};

#endif
