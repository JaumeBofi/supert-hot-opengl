#pragma once

#include <model.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <weapon.h>
#include <worldObject.h>

class Player : public WorldObject
{
private:	
	vector<Weapon*> _weapons;
	glm::vec3 _position;

public:

	Player(glm::vec3 position);	
	void AddWeapon(string modelName);
};