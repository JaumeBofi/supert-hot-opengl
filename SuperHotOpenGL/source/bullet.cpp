
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image_aug.h>
#include <assimp/postprocess.h>

#include <mesh.h>
#include <shader.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <cfloat>
#include <bullet.h>

using namespace std;


Bullet::Bullet() {
	_currentPosition = glm::vec3(0, 0, 0);
	_currentVelocity = glm::vec3(0, 0, 0);
	_shot = false;
	_isMoving = false;
	_acceleration = false;
}
Bullet::Bullet(glm::vec3 currentPosition, glm::vec3 currentVelocity, bool shot, bool isMoving, float acceleration) {
	_currentPosition = currentPosition;
	_currentVelocity = currentVelocity;
	_shot = shot;
	_isMoving = isMoving;
	_acceleration = acceleration;		
}
glm::mat4 Bullet::render() {
	glm::mat4 modBullet =
		glm::translate(glm::mat4(1.0f), glm::vec3(_currentPosition.x + 0.05, _currentPosition.y, _currentPosition.z)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(0.0005f, 0.0005f, 0.0005f))
		;	// it's a bit too big for our scene, so scale it down
	return modBullet;
}