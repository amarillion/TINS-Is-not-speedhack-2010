#include "Precompiled.hpp"
#include "Input.hpp"

std::map<unsigned int, Input::IdToInput> Input::mRegistrations;
bool Input::myEnabled = true;

Input::Input()
{

}

void Input::init()
{
    install_keyboard();
	install_joystick(JOY_TYPE_AUTODETECT);
}

bool Input::isPressed(unsigned int aId)
{
    if (mRegistrations.find(aId) == mRegistrations.end())
	{
		return false;
	}

	Input::IdToInput input = mRegistrations[aId];

	if (input.disabled)
	{
		return false;
	}

	// Check keys
	for (unsigned int i = 0; i < input.keys.size(); i++)
	{
		Input::Key k = input.keys[i];
		if (k.current && !k.old)
		{
			return true;
		}
	}

	// Check joypad buttons
	for (unsigned int i = 0; i < input.joypadButtons.size(); i++)
	{
		Input::JoypadButton j = input.joypadButtons[i];
		
		if (j.current && !j.old)
		{
			return true;
		}
	}

	// Check joypad sticks
	for (unsigned int i = 0; i < input.joypadSticks.size(); i++)
	{
		Input::JoypadStick j = input.joypadSticks[i];
		if (j.current && !j.old)
		{
			return true;
		}
	}

	return false;
}

bool Input::isReleased(unsigned int aId)
{
    if (mRegistrations.find(aId) == mRegistrations.end())
	{
		return false;
	}

	Input::IdToInput input = mRegistrations[aId];

	if (input.disabled)
	{
		return false;
	}

	// Check keys
	for (unsigned int i = 0; i < input.keys.size(); i++)
	{
		Input::Key k = input.keys[i];
		if (!k.current && k.old)
		{
			return true;
		}
	}

	// Check joypad buttons
	for (unsigned int i = 0; i < input.joypadButtons.size(); i++)
	{
		Input::JoypadButton j = input.joypadButtons[i];
		
		if (!j.current && j.old)
		{
			return true;
		}
	}

	// Check joypad sticks
	for (unsigned int i = 0; i < input.joypadSticks.size(); i++)
	{
		Input::JoypadStick j = input.joypadSticks[i];
		if (!j.current && j.old)
		{
			return true;
		}
	}

	return false;
}

bool Input::isHeld(unsigned int aId)
{
	if (mRegistrations.find(aId) == mRegistrations.end())
	{
		return false;
	}

	Input::IdToInput input = mRegistrations[aId];

	if (input.disabled)
	{
		return false;
	}

	// Check keys
	for (unsigned int i = 0; i < input.keys.size(); i++)
	{
		Input::Key k = input.keys[i];
		if (k.current)
		{
			return true;
		}
	}

	// Check joypad buttons
	for (unsigned int i = 0; i < input.joypadButtons.size(); i++)
	{
		Input::JoypadButton j = input.joypadButtons[i];
		
		if (j.current)
		{
			return true;
		}
	}

	// Check joypad sticks
	for (unsigned int i = 0; i < input.joypadSticks.size(); i++)
	{
		Input::JoypadStick j = input.joypadSticks[i];
		if (j.current)
		{
			return true;
		}
	}

	return false;
}

void Input::registerKey(unsigned int aId, int key)
{
	Key registeredKey;
	registeredKey.key = key;
	mRegistrations[aId].keys.push_back(registeredKey);
}

void Input::registerJoypadButton(unsigned int aId, int joy, int button)
{
	JoypadButton registeredJoypadButton;
	registeredJoypadButton.joy = joy;
	registeredJoypadButton.button = button;
	mRegistrations[aId].joypadButtons.push_back(registeredJoypadButton);
}

void Input::registerJoypadStick(unsigned int aId, int joy, int stick, int axis, int direction)
{
	JoypadStick registeredJoypadStick;
	registeredJoypadStick.joy = joy;
	registeredJoypadStick.stick = stick;
	registeredJoypadStick.axis = axis;
	registeredJoypadStick.direction = direction;
	mRegistrations[aId].joypadSticks.push_back(registeredJoypadStick);
}

void Input::disable(unsigned int aId)
{
	if (mRegistrations.find(aId) != mRegistrations.end())
	{
		mRegistrations[aId].disabled = true;
	}
}

void Input::enable(unsigned int aId)
{
	if (mRegistrations.find(aId) != mRegistrations.end())
	{
		mRegistrations[aId].disabled = false;
	}
}

void Input::disable()
{
	myEnabled = false;
}

void Input::enable()
{
	myEnabled = true;
}

void Input::onLogic()
{
	if (keyboard_needs_poll())
	{
		poll_keyboard();
	}

	if (!myEnabled)
		return;

	poll_joystick();

	for (std::map<unsigned int, Input::IdToInput>::iterator it = mRegistrations.begin(); it != mRegistrations.end(); it++)
	{
		IdToInput& idToInput = it->second;

		// Update keys
		for (unsigned int i = 0; i < idToInput.keys.size(); i++)
		{
			idToInput.keys[i].old = idToInput.keys[i].current;
			idToInput.keys[i].current = key[idToInput.keys[i].key] != 0; // != is used to surpress warnings.
		}

		// Update joypad buttons
		for (unsigned int i = 0; i < idToInput.joypadButtons.size(); i++)
		{
			idToInput.joypadButtons[i].old = idToInput.joypadButtons[i].current;
			int j = idToInput.joypadButtons[i].joy;
			int b = idToInput.joypadButtons[i].button;
			idToInput.joypadButtons[i].current = joy[j].button[b].b != 0; // != 0 is used to surpress warnings.
		}

		// Update joypad stick
		for (unsigned int i = 0; i < idToInput.joypadSticks.size(); i++)
		{
			idToInput.joypadSticks[i].old = idToInput.joypadSticks[i].current;
			int j = idToInput.joypadSticks[i].joy;
			int s = idToInput.joypadSticks[i].stick;
			int a = idToInput.joypadSticks[i].axis;
			int d = idToInput.joypadSticks[i].direction;
		
			if (joy[j].stick[s].axis[a].d1 == -1 && d < 0)
			{
				idToInput.joypadSticks[i].current = true;
			}
			else if (joy[j].stick[s].axis[a].d2 == -1 && d > 0)
			{
				idToInput.joypadSticks[i].current = true;
			}
			else if (joy[j].stick[s].axis[a].pos != 0 && d == 0)
			{
				idToInput.joypadSticks[i].current= true;
			}
			else
			{
				idToInput.joypadSticks[i].current = false;
			}
		}
	}
}

