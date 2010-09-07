#include "player.h"
#include "color.h"
#include "engine.h"
#include <math.h>

SpriteEx::SpriteEx (Game *game, SpriteType st, int x, int y, int _w, int _h, int _subtype) : Sprite (game, x, y)
{
	subtype = _subtype;
	dx = 0;
	dy = 0;
	air = true;
	spriteType = st;
	gravity = (st == ST_HEART || st == ST_HEART2 || st == ST_FOOD || st == ST_DROPZONE);
	ddy = gravity ? GRAVITY_ACC : 0;
	w = _w;
	h = _h;

	switch (spriteType)
	{
	case ST_PLAYER: setAnim(game->anims["raul"]); break;
	case ST_HEART: setAnim(game->anims["hs"]); break;
	case ST_FOOD:
		switch (subtype % 3)
		{
			case 0: setAnim(game->anims["corn"]); break;
			case 1: setAnim(game->anims["choco"]); break;
			case 2: setAnim(game->anims["tortilla"]); break;
		}
		break;
	case ST_ENEMY:
		switch (subtype % 3)
		{
			case 0: setAnim(game->anims["pepper1"]); break;
			case 1: setAnim(game->anims["pepper2"]); break;
			case 2: setAnim(game->anims["pepper3"]); break;
		}
		break;
	case ST_PLATFORM: break;
	case ST_DROPZONE: setAnim(game->anims["offering"]); break;
	case ST_HEART2: setAnim(game->anims["heart"]); break;
	case ST_FOLE: setAnim(game->anims["fole"]); break;
	}

	if (anim != NULL)
	{
		w = anim->sizex;
		h = anim->sizey;
	}

}

void SpriteEx::update()
{
	list<Sprite*>::iterator i;
	list<Sprite*> sprites = parent->getSprites();

	if (gravity)
	{
		dy += ddy;
		if (dy > MAX_Y) dy = MAX_Y;
		if (dy < -MAX_Y) dy = -MAX_Y;
	}

	y += dy;

	if (y + h > FLOOR)
	{
		y = FLOOR - h;
		onVBounce(ST_FLOOR, NULL);
		parent->getParent().playSample("Kg");
	}

	if (y < CEIL)
	{
		y = CEIL;
		onVBounce(ST_CEIL, NULL);
	}

	for (i = sprites.begin(); i != sprites.end(); ++i)
	{
		if (*i == this) continue;
		if (hasOverlap(*i))
		{
			SpriteEx * se = dynamic_cast<SpriteEx*>(*i);
			onVBounce(se->getSpriteType(), se);
		}
	}

	x += dx;

	if (x < 0) x += SCREEN_W;
	if (x >= SCREEN_W) x -= SCREEN_W;

	for (i = sprites.begin(); i != sprites.end(); ++i)
	{
		if (*i == this) continue;
		if (hasOverlap(*i))
		{
			SpriteEx * se = dynamic_cast<SpriteEx*>(*i);
			onHBounce(se->getSpriteType(), se);
		}
	}
}

void SpriteEx::onVBounce(SpriteType st, SpriteEx *s2)
{
	if (st == ST_CEIL || st == ST_FLOOR || st == ST_PLATFORM)
	{
		// if going down, we landed
		if (dy > 0)
		{
			air = false;
			ddy = 0;
			if (st == ST_PLATFORM)
			{
				y = s2->gety() - h - 1;
			}
		}
		dy = 0;
	}
}

void SpriteEx::onHBounce(SpriteType st, SpriteEx *s2)
{
}

void SpriteEx::onCol(Sprite *s)
{
	SpriteEx *se = dynamic_cast<SpriteEx *>(s);
	SpriteType st = se->getSpriteType();

	if (spriteType == ST_HEART2 && st == ST_DROPZONE)
	{
		parent->addScore(SCORE_DELIVER_HEART);
		parent->getParent().playSample("Plop");
		parent->decHeartCount();
		parent->dropHeart();
		parent->add (new ScoreText(parent, SCORE_DELIVER_HEART, getx(), gety()));
		kill();
	}
}

void Player::onCol(Sprite *s)
{
	SpriteEx *se = dynamic_cast<SpriteEx *>(s);

	switch (se->getSpriteType())
	{
	case ST_HEART:
		if (!loaded)
		{
			parent->addScore(SCORE_CATCH_HEART);
			parent->add (new ScoreText(parent, SCORE_CATCH_HEART, s->getx(), s->gety()));
			s->kill();
			loaded = true;
			state = 1;
		}
		break;
	case ST_FOOD:
		parent->getParent().playSample("Nom");
		parent->addScore(SCORE_FOOD);
		parent->add (new ScoreText(parent, SCORE_FOOD, s->getx(), s->gety()));
		s->kill();
		parent->decFoodCount();
		parent->incHp();
		break;
	case ST_ENEMY:
		s->kill();
		parent->getParent().playSample("Ugh");
		parent->playerHit();
		hitTimer = HIT_ANIM_LENGTH;
		state = 2;
		break;
	}
}


void SpriteEx::draw(BITMAP *buffer, int xofst, int yofst)
{
	int color;
	switch (spriteType)
	{
	case ST_PLAYER: color = YELLOW; break;
	case ST_HEART: color = RED; break;
	case ST_FOOD: color = BROWN; break;
	case ST_ENEMY: color = GREEN; break;
	case ST_PLATFORM: color = GREY; break;
	case ST_DROPZONE: color = BLACK; break;
	case ST_HEART2: color = ORANGE; break;
	case ST_FOLE: color = CYAN; break;
	}

	int x = getx() + xofst;
	int y = gety() + yofst;
	int time = parent->getParent().getCounter();

	if (anim != NULL)
	{
		anim->drawFrame (buffer, state, dir, time, x, y);

		if (x + w >= SCREEN_W)
		{
			anim->drawFrame (buffer, state, dir, time, x - SCREEN_W, y);
		}
	}
	else
	{
		rectfill (buffer, x, y, x + w, y + h, color);

		// if on screen edge, draw a second time shifted
		if (x + w >= SCREEN_W)
		{
			rectfill (buffer, x - SCREEN_W, y, x + w - SCREEN_W, y + h, color);
		}
	}

}

Player::Player (Game *game, int x, int y) : SpriteEx (game, ST_PLAYER, x, y)
{
	replay = game->getReplay();
	assert (replay);
	gravity = true;
	platform = NULL;
	loaded = false;
	state = 0;
	hitTimer = 0;
}

void Player::update()
{
	if (hitTimer > 0)
	{
		hitTimer--;
		if (hitTimer == 0)
		{
			state = loaded ? 1 : 0;
		}
	}

	if (replay->getState(btnUp))
	{
		ddy = -JET_ACC;
		air = true;
	}
	else
	{
		if (air)
		{
			ddy = GRAVITY_ACC;
		}
		else
		{
			ddy = 0;
		}
	}

	float speed = air ? AIR_HSPEED : LAND_HSPEED;
	if (replay->getState(btnLeft))
	{
		dx = -speed;
	}
	else if (replay->getState(btnRight))
	{
		dx = speed;
	}
	else
	{
		dx = 0;
	}
	SpriteEx::update();

	// check if we walked of the platform we're standing on.
	if (platform != NULL && !air)
	{
		if (!hasXOverlap(platform))
		{
			platform = NULL;
			air = true;
		}
	}

	// see if we should drop something
	if (loaded && hasXOverlap(parent->getDropzone()))
	{
		//TODO: replace 16 with heart width
		int newx = parent->getDropzone()->getx() + (parent->getDropzone()->getw() - 16) / 2;
		parent->add (new SpriteEx (parent, ST_HEART2, newx, y));
		loaded = false;
		state = 0;
	}

}

void Player::onHBounce(SpriteType st, SpriteEx *s2)
{
	if (st == ST_PLATFORM)
	{
		if (dx > 0)
		{
			x = s2->getx() - w - 1;
		}
		else
		{
			x = s2->getx() + s2->getw() + 1;
		}
		dx = 0;
	}
}

void Player::onVBounce(SpriteType st, SpriteEx *s2)
{
	if (st == ST_CEIL || st == ST_FLOOR || st == ST_PLATFORM)
	{
		// if going down, we landed
		if (dy > 0)
		{
			air = false;
			ddy = 0;
			if (st == ST_PLATFORM)
			{
				platform = s2;
				y = platform->gety() - h - 1;
			}
			dy = 0;
		}
		else
		{
			if (st == ST_PLATFORM)
			{
				platform = s2;
				y = platform->gety() + platform->geth() + 1;
			}
			dy = 0;
		}
	}
}


const int MAX_ENEMY_TYPE = 5;

Enemy::Enemy (Game *game, int x, int y, int _type) : SpriteEx (game, ST_ENEMY, x, y, 0, 0, _type)
{
	enemyType = (_type % MAX_ENEMY_TYPE);
	gravity = false;
	floorKills = false;
	hbounceKills = false;
	period = 1;
	vsign = 1.0;
	hsign = (rand() % 100 > 50) ? 1.0 : -1.0;

	switch (enemyType)
	{
	case 0:
		dx = 2;
		dy = 4;
		hbounceKills = true;
		break;
	case 1:
		dx = 3;
		dy = 3;
		break;
	case 2:
		dx = 4;
		dy = 1;
		floorKills = true;
		break;
	case 3:
		dx = 3;
		dy = 0;
		period = 300;
		break;
	case 4:
		dx = 2;
		dy = 4;
		period = 150;
		break;
	}

	dx *= hsign;
}

void Enemy::update()
{
	phase = (phase + 1) % period;

	switch (enemyType)
	{
	case 3:
		{
			dy = 2.5 * vsign * sin(float(phase) * 6.282f / float(period));
		}
		break;
	case 4:
		{
			if (phase == 0) dy = -dy;
		}
		break;
	}

	SpriteEx::update();
}

void Enemy::kill()
{
	parent->enemyKilled();
	Sprite::kill();
}

void Enemy::onVBounce(SpriteType st, SpriteEx *s2)
{
	if (floorKills && dy > 0)
	{
		kill();
		return;
	}

	if (st == ST_PLATFORM || st == ST_ENEMY || st == ST_FLOOR || st == ST_CEIL)
	{
		// undo crossing the line
		y -= dy;

		// reverse direction
		dy = -dy;
		vsign = -vsign;
	}
}

void Enemy::onHBounce(SpriteType st, SpriteEx *s2)
{
	if (hbounceKills && st == ST_PLATFORM)
	{
		kill();
		return;
	}

	if (st == ST_PLATFORM || st == ST_ENEMY)
	{
		// undo crossing the line
		x -= dx;

		// reverse direction
		dx = -dx;
	}
}

void ScoreText::draw(BITMAP *buffer, int xofst, int yofst)
{
	int x = getx();
	int y = gety();
	draw_sprite (buffer, cache, x, y);

	if (x + cache->w > SCREEN_W)
	{
		draw_sprite (buffer, cache, x - SCREEN_W, y);
	}

}

ScoreText::ScoreText(Game * game, int score, int x, int y) : SpriteEx (game, ST_SCORE, x, y)
{
	dx = 0;
	dy = -0.5;
	timer = 100;

	ALFONT_FONT *smallFont = parent->smallfont;
	cache = create_bitmap (100, 20);
	clear_to_color (cache, MAGIC_PINK);

	alfont_textprintf_ex (cache, smallFont, 1, 1, BLACK, -1, "%i", score);
	alfont_textprintf_ex (cache, smallFont, 2, 1, BLACK, -1, "%i", score);
	alfont_textprintf_ex (cache, smallFont, 0, 0, WHITE, -1, "%i", score);
	alfont_textprintf_ex (cache, smallFont, 1, 0, WHITE, -1, "%i", score);
}

ScoreText::~ScoreText()
{
	destroy_bitmap (cache);
}


