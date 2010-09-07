#ifndef GAME_H
#define GAME_H

#include "layer.h"
#include <allegro.h>
#include <list>
#include <map>
#include "layer.h"
#include <vector>
#include <alfont.h>
#include "level.h"
#include "replay.h"

class SpriteLayer;
class Engine;
class Resources;
class Game;
class Anim;

class Player;
class SpriteEx;

class Sprite
{
	friend class Game;
	friend class Player;

protected:
	float x;
	float y;
	int w;
	int h;
	Game* parent;
	virtual void kill ();
	Anim *anim;
	int state;
	int dir;
	int animStart;
private:
	int waitTimer;
		
	void setParent (Game *_parent);
	bool alive;
	bool visible;

public:
	Sprite (Game * game, int amx, int amy);
	void setAnim (Anim *_anim) { anim = _anim; }
	virtual ~Sprite () {}
	
	bool isAlive() const { return alive; } // if not, scheduled to be killed
	bool isVisible() const { return visible; }
			
	virtual void draw(BITMAP *buffer, int xofst, int yofst);
	virtual void onCol(Sprite *s) {};

	int getx () const { return x; }
	int gety () const { return y; }
	int getw () const { return w; }
	int geth () const { return h; }
	
	virtual void update();
	bool hasOverlap (Sprite *s2);
	bool hasXOverlap (Sprite *s2);
};

enum GameState
{
	GS_LEVEL_INTRO, GS_PAUSE, GS_GAMEOVER, GS_PLAYING, GS_NEXT_LEVEL};

// general constants
const int START_HP = 3;
const int START_LIVES = 3;
const int DROPS_NEEDED = 6; // number of drops before going to next level

// player movement constants
const float MAX_Y = 6.0;
const int FLOOR = 640;
const int CEIL = 0;
const float GRAVITY_ACC = 0.4;
const float JET_ACC = 0.4;
const float AIR_HSPEED = 6.0;
const float LAND_HSPEED = 3.0;
const int DEFAULT_SPRITE_W = 48;
const int DEFAULT_SPRITE_H = 48;

// score constants
const int SCORE_CATCH_HEART = 100;
const int SCORE_DELIVER_HEART = 200;
const int SCORE_FOOD = 300;

const int ANIM_NUM = 8;
const int HIT_ANIM_LENGTH = 20;

class Game : public Layer
{
	std::list <Sprite*> sprites;
	BITMAP *bgImage;

	// per-level, initialized in initLevel
	Player *player;
	SpriteEx *dropzone;
	int dropped;

	int heartCount;
	int foodCount;
	int enemyCount;
	int enemyTimer;
	int foodTimer;
	int heartTimer;
	LevelData currentLevelData;

	// per-game, initialized in init
	bool gameover;
	int score;
	int lives;
	int hp;
	int currentLevel;

	// per session
	int hi;

	// state
	GameState state;
	IReplay *replay; //TODO: use shared_ptr

	int stateTimer;

	void addEnemy();
	void generatedObjects();
	void updateObjects();

	Button bEsc;
	Button bSpace;
public:
	static std::map<std::string, Anim*> anims;
	ALFONT_FONT *gamefont;	
	ALFONT_FONT *smallfont;

	virtual ~Game() { killAll(); }
	void killAll();
	virtual void done();
	Game (Engine &engine);	
	
	void init();
	void initLevel();
	void initGame(IReplay *_replay);

	void dropHeart();
	void playerHit();
	void enemyKilled() { enemyCount--; }
	void decHeartCount() { heartCount--; }
	void decFoodCount() { foodCount--; }
	std::list<Sprite*> &getSprites() { return sprites; }
	SpriteEx *getDropzone() { return dropzone; }
	void add(Sprite *o);
	void addScore(int amount);
	void incHp() { if (hp < START_HP) hp++; }
	IReplay *getReplay() { return replay; }

	virtual void update();	
	virtual void draw(BITMAP *dest);
};

#endif
