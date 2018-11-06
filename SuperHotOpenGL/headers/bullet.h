#pragma once

#include <model.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <bullet.h>
#include <vector>
#include <worldObject.h>

class Bullet : public WorldObject
{
private:
	glm::vec3 _currentPosition;
	glm::vec3 _currentVelocity;
	float _acceleration;
	bool _shot;
	bool _isMoving;	

public:
	Bullet();
	Bullet(glm::vec3, glm::vec3, bool, bool, float);
	glm::mat4 render();

};