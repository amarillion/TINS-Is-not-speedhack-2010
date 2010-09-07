#include <allegro5/allegro5.h>
#include "t3f.h"
#include "controller.h"

static char t3f_binding_return_name[256] = {0};

T3F_CONTROLLER * t3f_create_controller(int bindings)
{
	T3F_CONTROLLER * cp;
	
	cp = malloc(sizeof(T3F_CONTROLLER));
	if(!cp)
	{
		return NULL;
	}
	memset(cp, 0, sizeof(T3F_CONTROLLER));
	cp->bindings = bindings;
	return cp;
}

void t3f_destroy_controller(T3F_CONTROLLER * cp)
{
	free(cp);
}

const char * t3f_get_controller_binding_name(T3F_CONTROLLER * cp, int binding)
{
	const char * name;
	switch(cp->binding[binding].type)
	{
		case T3F_CONTROLLER_BINDING_KEY:
		{
			sprintf(t3f_binding_return_name, "Keyboard: %s", al_keycode_to_name(cp->binding[binding].button));
			return t3f_binding_return_name;
		}
		case T3F_CONTROLLER_BINDING_JOYSTICK_BUTTON:
		{
			if(t3f_joystick[cp->binding[binding].joystick])
			{
				name = al_get_joystick_button_name(t3f_joystick[cp->binding[binding].joystick], cp->binding[binding].button);
				sprintf(t3f_binding_return_name, "%s (%d): Button %s", al_get_joystick_name(t3f_joystick[cp->binding[binding].joystick]), cp->binding[binding].joystick, name ? name : "???");
			}
			else
			{
				sprintf(t3f_binding_return_name, "N/A");
			}
			return t3f_binding_return_name;
		}
		case T3F_CONTROLLER_BINDING_JOYSTICK_AXIS:
		{
			if(t3f_joystick[cp->binding[binding].joystick])
			{
				name = al_get_joystick_axis_name(t3f_joystick[cp->binding[binding].joystick], cp->binding[binding].stick, cp->binding[binding].axis);
				sprintf(t3f_binding_return_name, "%s (%d): Axis %s%s", al_get_joystick_name(t3f_joystick[cp->binding[binding].joystick]), cp->binding[binding].joystick, name ? name : "???", (cp->binding[binding].flags & T3F_CONTROLLER_FLAG_AXIS_NEGATIVE) ? "-" : "+");
			}
			else
			{
				sprintf(t3f_binding_return_name, "N/A");
			}
			return t3f_binding_return_name;
		}
	}
	t3f_binding_return_name[0] = 0;
	return t3f_binding_return_name;
}

bool t3f_bind_controller(T3F_CONTROLLER * cp, int binding)
{
	ALLEGRO_EVENT_QUEUE * queue;
	ALLEGRO_EVENT event;
	float ppos = 0.0;
	int i;
	
	queue = al_create_event_queue();
	if(!queue)
	{
		return false;
	}
	if(t3f_flags & T3F_USE_KEYBOARD)
	{
		al_register_event_source(queue, al_get_keyboard_event_source());
	}
	if(t3f_flags & T3F_USE_MOUSE)
	{
		al_register_event_source(queue, al_get_mouse_event_source());
	}
	if(t3f_flags & T3F_USE_JOYSTICK)
	{
		for(i = 0; i < T3F_MAX_JOYSTICKS; i++)
		{
			t3f_joystick[i] = al_get_joystick(i);
			if(t3f_joystick[i])
			{
				al_register_event_source(queue, al_get_joystick_event_source(t3f_joystick[i]));
				al_get_joystick_state(t3f_joystick[i], &t3f_joystick_state[i]); // read initial state
			}
		}
	}
	while(1)
	{
		al_wait_for_event(queue, &event);
		switch(event.type)
		{
			/* key was pressed or repeated */
			case ALLEGRO_EVENT_KEY_DOWN:
			case ALLEGRO_EVENT_KEY_REPEAT:
			{
				if(event.keyboard.keycode != ALLEGRO_KEY_ESCAPE)
				{
					cp->binding[binding].type = T3F_CONTROLLER_BINDING_KEY;
					cp->binding[binding].button = event.keyboard.keycode;
					al_destroy_event_queue(queue);
					return true;
				}
				else
				{
					al_destroy_event_queue(queue);
					return false;
				}
			}
			case ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN:
			{
				cp->binding[binding].type = T3F_CONTROLLER_BINDING_JOYSTICK_BUTTON;
				cp->binding[binding].joystick = t3f_get_joystick_number(event.joystick.source);
				cp->binding[binding].stick = event.joystick.stick;
				cp->binding[binding].button = event.joystick.button;
				al_destroy_event_queue(queue);
				return true;
			}
			case ALLEGRO_EVENT_JOYSTICK_AXIS:
			{
//				if(ppos != 0.0)
				{
					if(event.joystick.pos < -T3F_CONTROLLER_AXIS_THRESHOLD && t3f_joystick_state[t3f_get_joystick_number(event.joystick.source)].stick[event.joystick.stick].axis[event.joystick.axis] >= -T3F_CONTROLLER_AXIS_THRESHOLD)
					{
						cp->binding[binding].type = T3F_CONTROLLER_BINDING_JOYSTICK_AXIS;
						cp->binding[binding].joystick = t3f_get_joystick_number(event.joystick.source);
						cp->binding[binding].stick = event.joystick.stick;
						cp->binding[binding].axis = event.joystick.axis;
						cp->binding[binding].flags = T3F_CONTROLLER_FLAG_AXIS_NEGATIVE;
						al_destroy_event_queue(queue);
						return true;
					}
					else if(event.joystick.pos > T3F_CONTROLLER_AXIS_THRESHOLD && t3f_joystick_state[t3f_get_joystick_number(event.joystick.source)].stick[event.joystick.stick].axis[event.joystick.axis] <= T3F_CONTROLLER_AXIS_THRESHOLD)
					{
						cp->binding[binding].type = T3F_CONTROLLER_BINDING_JOYSTICK_AXIS;
						cp->binding[binding].joystick = t3f_get_joystick_number(event.joystick.source);
						cp->binding[binding].stick = event.joystick.stick;
						cp->binding[binding].axis = event.joystick.axis;
						cp->binding[binding].flags = T3F_CONTROLLER_FLAG_AXIS_POSITIVE;
						al_destroy_event_queue(queue);
						return true;
					}
				}
				if(event.joystick.pos >= -T3F_CONTROLLER_AXIS_THRESHOLD && event.joystick.pos <= T3F_CONTROLLER_AXIS_THRESHOLD)
				{
					ppos = event.joystick.pos;
				}
				if(al_get_joystick_stick_flags(event.joystick.source, event.joystick.stick) & ALLEGRO_JOYFLAG_DIGITAL)
				{
					ppos = 0.1;
				}
				break;
			}
			case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
			{
				cp->binding[binding].type = T3F_CONTROLLER_BINDING_MOUSE_BUTTON;
				cp->binding[binding].button = event.mouse.button;
				al_destroy_event_queue(queue);
				return true;
			}
		}
	}
	return false;
}

/* see which buttons are held */
void t3f_read_controller(T3F_CONTROLLER * cp)
{
	int i;
	
	for(i = 0; i < cp->bindings; i++)
	{
		switch(cp->binding[i].type)
		{
			case T3F_CONTROLLER_BINDING_KEY:
			{
				if(t3f_key[cp->binding[i].button])
				{
					cp->state[i].down = true;
				}
				else
				{
					cp->state[i].down = false;
				}
				break;
			}
			case T3F_CONTROLLER_BINDING_JOYSTICK_BUTTON:
			{
				if(t3f_joystick_state[cp->binding[i].joystick].button[cp->binding[i].button])
				{
					cp->state[i].down = true;
				}
				else
				{
					cp->state[i].down = false;
				}
				break;
			}
			case T3F_CONTROLLER_BINDING_JOYSTICK_AXIS:
			{
				bool held = false;
				cp->state[i].pos = t3f_joystick_state[cp->binding[i].joystick].stick[cp->binding[i].stick].axis[cp->binding[i].axis];
				if((cp->binding[i].flags & T3F_CONTROLLER_FLAG_AXIS_NEGATIVE) && t3f_joystick_state[cp->binding[i].joystick].stick[cp->binding[i].stick].axis[cp->binding[i].axis] < -T3F_CONTROLLER_AXIS_THRESHOLD)
				{
					held = true;
				}
				else if((cp->binding[i].flags & T3F_CONTROLLER_FLAG_AXIS_POSITIVE) && t3f_joystick_state[cp->binding[i].joystick].stick[cp->binding[i].stick].axis[cp->binding[i].axis] > T3F_CONTROLLER_AXIS_THRESHOLD)
				{
					held = true;
				}
				if(held)
				{
					cp->state[i].down = true;
				}
				else
				{
					cp->state[i].down = false;
				}
				break;
			}
		}
	}
}

/* update pressed/released variables */
void t3f_update_controller(T3F_CONTROLLER * cp)
{
	int i;
	
	for(i = 0; i < cp->bindings; i++)
	{
		cp->state[i].was_held = cp->state[i].held;
		if(cp->state[i].down)
		{
			if(!cp->state[i].was_held)
			{
				cp->state[i].pressed = true;
				cp->state[i].released = false;
			}
			else
			{
				cp->state[i].pressed = false;
			}
			cp->state[i].held = true;
		}
		else
		{
			if(cp->state[i].was_held)
			{
				cp->state[i].released = true;
			}
			else
			{
				cp->state[i].released = false;
			}
			cp->state[i].pressed = false;
			cp->state[i].held = false;
		}
	}
}

void t3f_clear_controller_state(T3F_CONTROLLER * cp)
{
	int i;
	
	for(i = 0; i < cp->bindings; i++)
	{
		cp->state[i].down = false;
		cp->state[i].held = false;
		cp->state[i].was_held = false;
		cp->state[i].pressed = false;
		cp->state[i].released = false;
	}
}
