#pragma once

class Input
{
public:
	static void init();
	static bool isPressed(unsigned int aId);
    static bool isReleased(unsigned int aId);
	static bool isHeld(unsigned int aId);
	static void registerKey(unsigned int aId, int key);
	static void registerJoypadButton(unsigned int aId, int joy, int button);
	static void registerJoypadStick(unsigned int aId, int joy, int stick, int axis, int direction);
	static void disable(unsigned int aId);
	static void enable(unsigned int aId);
	static void onLogic();
	static void disable();
	static void enable();

private:
	Input();

	class Source
	{
	public:
		Source() : old(false), current(false) {}
		bool old;
		bool current;
	};
	class Key: public Source
	{
	public:
		int key;
	};
	class JoypadButton: public Source
	{
	public:
		int joy;
		int button;
	};
	class JoypadStick: public Source
	{
	public:
		int joy;
		int stick;
		int axis;
		int direction;
	};

	class IdToInput
	{
	public:
		IdToInput() : disabled(false) {}
		std::vector<Key> keys;
		std::vector<JoypadButton> joypadButtons;
		std::vector<JoypadStick> joypadSticks;
		bool disabled;
	};

	static std::map<unsigned int, IdToInput> mRegistrations;
	static bool myEnabled;
};