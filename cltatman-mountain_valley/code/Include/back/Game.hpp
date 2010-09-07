#ifndef GAME_HPP
#define GAME_HPP

#include <Keyboard.hpp>
#include <Mouse.hpp>
#include <Level.hpp>
#include <Camera.hpp>
#include <Timer.hpp>
#include <Editor.hpp>
#include <EntityFactory.hpp>

#include <string>


class Game
{
  public:
    Game( unsigned w, unsigned h, const std::string& title, bool fullscreen = true );

    void initialize();
    void run();
    void draw();
    void update();
    void drawBill( const std::string&, double,double );

    bool shouldQuit,
         isAngel;

    double width,height,
           angelAngle;

    unsigned long endTime;

    Timer timer;

    Keyboard keyboard;
    Mouse mouse;

    Level level;
    Camera camera;

    Editor editor;

    EntityFactory entityFactory;
};

extern Game game;

#endif // GAME_HPP
