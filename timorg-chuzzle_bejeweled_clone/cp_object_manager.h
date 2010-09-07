#ifndef CP_OBJECT_MANAGER_H
#define CP_OBJECT_MANAGER_H

#include <allegro.h>

#include <list>
#include <algorithm>

#include "counted_ptr.h"


template <class T>
class IS_DEAD
{
  public:
    bool operator()(counted_ptr<T> o)
    {
        return !o->is_alive();
    }
};

template <class T>
struct DO_RENDER
{
  public:
    DO_RENDER() : buffer(NULL) {}
    void bind_buffer(BITMAP *b)
    {
        buffer = b;
    }
    void operator()(counted_ptr<T> o)
    {
        #ifdef DEBUGMODE
        if(buffer == NULL)
        {
            allegro_message("You didn't set the buffer for an instance of DO_RENDER<T>");
            exit(1);
        }
        #endif
        o->render(buffer);
    }
  private:
    BITMAP *buffer;
};

template <class T>
class DO_LOGIC
{
  public:
    void operator()(counted_ptr<T> o)
    {
        o->logic();
    }
};

template <class T>
class CP_OBJECT_MANAGER
{
  public:

    void add(counted_ptr<T> o)
    {
        items.push_back(o);
    }

    void render(BITMAP *buffer)
    {
        do_render.bind_buffer(buffer);
        for_each(items.begin(), items.end(), do_render);
    }

    void logic()
    {
        for_each(items.begin(), items.end(), do_logic);
    }

    void prune()
    {
        items.remove_if(is_dead);
    }

    int count()
    {
        return items.size();
    }

    std::list<counted_ptr<T> > items;
  private:
    IS_DEAD<T> is_dead;
    DO_RENDER<T> do_render;
    DO_LOGIC<T> do_logic;
};




#endif
