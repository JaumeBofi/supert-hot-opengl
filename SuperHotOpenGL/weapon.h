#pragma once

#include <model.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <bullet.h>
#include <vector>
#include <string.h>
#include <worldObject.h>

using namespace std;

class Weapon : public  WorldObject
{
private:
	Model* _model;
	int _clipSize;
	vector<Bullet*> _currentBullets;
	vector<Bullet*> _shotBullets;

public:
	Weapon(string modelName,int clipSize);

};