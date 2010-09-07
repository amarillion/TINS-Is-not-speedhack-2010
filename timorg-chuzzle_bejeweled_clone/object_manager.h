#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#include <allegro.h>

#include <list>
#include <algorithm>


template <class T>
class IS_DEAD
{
  public:
    bool operator()(T& o)
    {
        return !o.is_alive();
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
    void operator()(T& o)
    {
        #ifdef DEBUGMODE
        if(buffer == NULL)
        {
            allegro_message("You didn't set the buffer for an instance of DO_RENDER<T>");
            exit(1);
        }
        #endif
        o.render(buffer);
    }
  private:
    BITMAP *buffer;
};

template <class T>
class DO_LOGIC
{
  public:
    void operator()(T& o)
    {
        o.logic();
    }
};

template <class T>
class OBJECT_MANAGER
{
  public:

    void add(T o)
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

  private:
    std::list<T> items;
    IS_DEAD<T> is_dead;
    DO_RENDER<T> do_render;
    DO_LOGIC<T> do_logic;
};


#endif
