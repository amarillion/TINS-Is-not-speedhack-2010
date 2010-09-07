#include "Screen.hpp"

class Font;
class Battle;

class GameOverScreen : public Screen {
public:
	GameOverScreen();
	virtual void onLogic();
	virtual void onDraw(BITMAP* buffer);
private:
	Font *myFont;
};