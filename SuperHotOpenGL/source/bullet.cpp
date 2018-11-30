
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
	_currentDirection = glm::vec3(0, 0, 0);
	_shot = false;
	_isMoving = false;	
	glm::mat4 initialModelMat = glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.05f, 0.05f));
	InitialModelMat(initialModelMat);
}

Bullet::Bullet(Model* bulletMesh,glm::vec3 currentPosition, glm::vec3 currentDirection, bool shot, bool isMoving,float velocity) {
	Mesh(bulletMesh);			
	_currentPosition = currentPosition + 0.005f*currentDirection;
	_currentDirection = currentDirection;
	_velocity = velocity;
	_shot = shot;
	_isMoving = isMoving;	
	glm::mat4 initialModelMat = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.00002f, 0.00002f, 0.00002f));
	InitialModelMat(initialModelMat);
	ModelMat(initialModelMat);
}

void Bullet::UpdatePosicion()
{
	_currentPosition += _currentDirection * _velocity ;
	ModelMat(glm::translate(glm::mat4(1.0f), _currentPosition)*InitialModelMat());
	Mesh()->UpdateCollisionModel(glm::translate(glm::mat4(1.0f), _currentPosition)*InitialModelMat());
}