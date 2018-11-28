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
	glm::vec3 _currentDirection;	
	float _velocity;
	bool _shot;		
	bool _isMoving;	
	
	glm::mat4 _initialModel;

public:
	Bullet();
	Bullet(Model* bulletMesh,glm::vec3, glm::vec3, bool, bool,float);
	void UpdatePosicion();
};