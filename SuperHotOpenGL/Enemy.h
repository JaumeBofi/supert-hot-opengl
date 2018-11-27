#pragma once
#pragma once

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
#include <model.h>
#include <list>

#include "Bala.h"
using namespace std;




class Enemy
{
private:

	glm::vec3 initialPosition;
	glm::vec3 finalPosition;	
	std::list<Bala> listaBalas;


public:

	Enemy();
	Enemy(glm::vec3, glm::vec3);
	glm::mat4 render(Model* modelPistola, Model* modelBala, Shader* mainShader, float playerX, float playerY);
	void Enemy::renderPistol(Model* modelPistol, Model* modelBullet, Shader* mainShader);
	void Enemy::renderBullet(Model* modelBullet, Shader* mainShader);
	void Enemy::updateBulletsPosition(float increment);
	void Enemy::addBullet(glm::vec3);
	//void moveEnemy(float increment);
	//void shoot(glm::vec3 playerPosition);

};



