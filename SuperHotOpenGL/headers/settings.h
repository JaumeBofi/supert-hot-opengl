#pragma once
#include <string>

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;
const std::string SHADERSDIRECTORY = "..\\SuperHotOpenGL\\source\\";


class Settings
{

private:
	unsigned int _scrWidth = SCR_WIDTH;
	unsigned int _scrHeight = SCR_HEIGHT;
	std::string _shadersDirectory = SHADERSDIRECTORY;

public:
	
	Settings(unsigned int scrHeight = SCR_HEIGHT, unsigned int scrWidth = SCR_WIDTH, std::string shadersDirectory = SHADERSDIRECTORY);
	
	unsigned int ScrWidth() const { return _scrWidth; }
	void ScrWidth(unsigned int val) { _scrWidth = val; }
	unsigned int ScrHeight() const { return _scrHeight; }
	void ScrHeight(unsigned int val) { _scrHeight = val; }

	std::string ShadersDirectory() const { return _shadersDirectory; }
	void ShadersDirectory(std::string val) { _shadersDirectory = val; }
};



