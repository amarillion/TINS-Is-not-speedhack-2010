#include "Precompiled.hpp"
#include "ScreenManager.hpp"
#include "Screen.hpp"

std::vector<Screen*> ScreenManager::mScreens;
std::vector<Screen*> ScreenManager::mScreensToDelete;
Screen* ScreenManager::mScreenToExit = 0;

ScreenManager::ScreenManager()
{
}

void ScreenManager::init()
{
}

void ScreenManager::destroy()
{
	for (unsigned int i = 0; i < mScreens.size(); i++)
	{
		delete mScreens[i];
	}
}

Screen* ScreenManager::getTop()
{
	if (mScreens.size() == 0)
		return 0;
	return mScreens[mScreens.size() - 1];
}

Screen* ScreenManager::getActive()
{
    for (unsigned int i = mScreens.size() - 1; i >= 0; i--)
    {
        Screen* s = mScreens[i];

		if (s->getState() == Screen::Active)
        {
            return s;
        }
    }
	
	return 0;
}

bool ScreenManager::isEmpty()
{
	return mScreens.empty();
}

void ScreenManager::add(Screen* screen)
{
	Screen* top = getTop();
	if (top != 0)
	{
		if (!screen->isDialog())
		{
			top->hide();
		}
	}

	screen->setState(Screen::Active);
	mScreens.push_back(screen);
}

void ScreenManager::exit(Screen* screen)
{
	Screen* active = getActive();

    if (screen != active)
    {
		throw std::string("Screen is not active!");
    }

	if (mScreenToExit != 0)
	{
		throw std::string("Screen to exit is not set!");
	}

	mScreenToExit = screen;
}

void ScreenManager::onLogic()
{
	for (unsigned int i = 0; i < mScreens.size(); i++)
	{
		if (mScreens[i]->getState() == Screen::Active)
		{
			mScreens[i]->onLogic();
		}
	}

	if (mScreenToExit != 0)
	{
		Screen* active = getActive();
		active->hide();

		for (unsigned int i = 0; i < mScreens.size(); i++)
		{
			if (mScreens[i] == mScreenToExit)
			{
				if (i > 0)
				{
					mScreens[i - 1]->setState(Screen::Active);
				}

				mScreensToDelete.push_back(mScreenToExit);
				mScreens.erase(mScreens.begin() + i);
				break;
			}
		}
		mScreenToExit = 0;
	}

	for (unsigned int i = 0; i < mScreensToDelete.size(); i++)
	{
		delete mScreensToDelete[i];
	}
	mScreensToDelete.clear();
}

void ScreenManager::draw(BITMAP* buffer)
{
	for (unsigned int i = 0; i < mScreens.size(); i++)
	{
		if (mScreens[i]->getState() != Screen::Hidden)
		{
			mScreens[i]->onDraw(buffer);
		}
	}
}