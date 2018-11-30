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
#include <weapon.h>
using namespace std;




class Enemy : public WorldObject
{
private:

	glm::vec3 initialPosition;
	glm::vec3 finalPosition;
	std::list<Bala> listaBalas;
	bool followPlayer;
	bool dead;
	float fireTimer;
	Weapon* weapon;

public:

	Enemy();
	Enemy(string modelName,Weapon*, glm::vec3);
	Enemy(string modelName,Weapon*,glm::vec3, bool);

	void Enemy::updateBulletsPosition();
	void Enemy::addBullet(glm::vec3);
	void Enemy::update(glm::vec3);
	void Enemy::renderEnemyBullet(Shader* mainShader);
	void Enemy::renderEnemyWeapon(float angle, Shader* mainShader);
	void Enemy::setWeapon(Weapon*);
	void Enemy::renderEnemy(glm::vec3 playerPosition, Shader* mainShader);
	void Enemy::fire(glm::vec3 positionPlayer);
	Weapon* Enemy::CurrentWeapon();
	bool Enemy::isDead();
	void Enemy::setDead();
};



