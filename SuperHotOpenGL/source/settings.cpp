#include <iostream>

#define SCR_WIDTH 800
#define SCR_HEIGHT 600


class Settings
{
	
public:
	unsigned int _scrWidth = SCR_WIDTH;
	unsigned int _scrHeight = SCR_HEIGHT;

	Settings()
	{

	}

	Settings(unsigned int scrHeight = SCR_HEIGHT, unsigned int scrWidth = SCR_WIDTH)
	{
		_scrHeight = scrHeight;
		_scrWidth = scrWidth;
	}
};





