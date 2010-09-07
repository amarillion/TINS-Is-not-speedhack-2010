#ifndef MOUSE_BUTTON_H
#define MOUSE_BUTTON_H

class MOUSE_BUTTON
{
  public:
    MOUSE_BUTTON(int b);
    bool operator()();

  public:
   int button;
   bool button_down;
};

extern MOUSE_BUTTON left_mouse;


#endif
