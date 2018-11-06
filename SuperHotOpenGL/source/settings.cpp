#include <iostream>
#include <settings.h>



Settings::Settings(unsigned int scrHeight, unsigned int scrWidth, std::string shadersDirectory)
{
	ScrWidth(scrWidth);
	ScrHeight(scrHeight);
	ShadersDirectory(shadersDirectory);
}


