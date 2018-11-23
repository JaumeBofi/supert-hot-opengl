#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image_aug.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
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
#include "Enemy.h"

using namespace std;

Enemy::Enemy() {
	this->initialPosition = glm::vec3(0, 0, 0);
	this->finalPosition = glm::vec3(0, 0, 0);
	
}
Enemy::Enemy(glm::vec3 initialPosition, glm::vec3 finalPosition) {
	this->initialPosition = initialPosition;
	this->finalPosition = finalPosition;
	
}
glm::mat4 Enemy::initialRender() {
	printf("-----\n");
	printf("\n\n");

	printf("Enemy InitialPosition x: %f , Enemy FinalPosition y: %f", this->initialPosition, this->finalPosition);
	//glm::vec3 nuevo = this->direccion*0.005f;
	//this->posicionActual = nuevo;
	glm::mat4 enemyModel =
		glm::translate(glm::mat4(1.0f), glm::vec3(this->initialPosition.x, this->initialPosition.y, this->initialPosition.z)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(0.00005f, 0.00005f, 0.00005f))
		;	// it's a bit too big for our scene, so scale it down
			//this->posicionActual.x -= 0.002f;
	
	return enemyModel;
}

//void Bala::actualizarPosicion(float increment) {
//	//this->posicionActual += this->direccion*0.5f;
//	this->posicionActual += this->direccion*increment;
//}