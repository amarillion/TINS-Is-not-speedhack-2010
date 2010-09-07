#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP


class Component;


typedef Component* (*ComponentBuilder)();


template <class T>
inline Component* makeComponent()
{
  return new T();
}

#define R_(a) game.entityFactory.componentRepo[#a] = &makeComponent<a>
#define REGISTER(a) static bool r = R_(a)

#endif
