#include <GL/glew.h>
#include <GL/freeglut.h>
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
#include <model.h>
#include <weapon.h>
using namespace std;

const float angleFactor = 70.0f;
const float pistolPosYDiff = 0.0090f;
const float movSpeed = 0.002f;

Enemy::Enemy() {
	this->initialPosition = glm::vec3(0, 0, 0);
	this->finalPosition = glm::vec3(0, 0, 0);
}
Enemy::Enemy(string modelName,Weapon* weapon,glm::vec3 initialPosition) {
	Mesh(new Model(modelName));
	Mesh()->ComputeData();
	this->initialPosition = initialPosition;
	this->finalPosition = finalPosition;
	this->followPlayer = false;
	this->weapon = weapon;
	glm::mat4 enemyModel =
		glm::translate(glm::mat4(1.0f), glm::vec3(this->initialPosition.x, this->initialPosition.y, this->initialPosition.z)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(0.00008f, 0.00008f, 0.00008f));

	InitialModelMat(enemyModel);
}

Enemy::Enemy(string modelName,Weapon* weapon,glm::vec3 initialPosition,bool followPlayer) {
	Mesh(new Model(modelName));
	Mesh()->ComputeData();
	this->initialPosition = initialPosition;
	this->finalPosition = finalPosition;
	this->followPlayer = followPlayer;
	this->weapon = weapon;
	glm::mat4 enemyModel =
		glm::translate(glm::mat4(1.0f), glm::vec3(this->initialPosition.x, this->initialPosition.y, this->initialPosition.z)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(0.00008f, 0.00008f, 0.00008f));

	InitialModelMat(enemyModel);
}

void Enemy::setWeapon(Weapon* weapon){
	this->weapon = weapon;
}

void Enemy::renderEnemyBullet(Shader* mainShader){
	std::vector<Bullet*>::iterator bullet;
	for (bullet = weapon->CurrentBullets()->begin(); bullet != weapon->CurrentBullets()->end(); ++bullet) {

		try
		{
			(*bullet)->UpdatePosicion();
			(*bullet)->render(mainShader);
		}
		catch (const std::exception &exc)
		{
			printf(exc.what());
		}
	}
}

void Enemy::renderEnemyWeapon(float angle, Shader* mainShader){
	glm::mat4 modWeapon =
		glm::translate(glm::mat4(1.0f), glm::vec3(this->initialPosition.x, this->initialPosition.y + pistolPosYDiff, this->initialPosition.z)) * //llevar pistola junto a la posicion de la camara
		glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f + angle*angleFactor), glm::vec3(0.0f, 1.0f, 0.0f));  //con rotacion para seguir al jugador
	this->weapon->UpdatePosition(modWeapon);
	this->weapon->render(mainShader);
}

void Enemy::renderEnemy(glm::vec3 playerPosition, Shader* mainShader){
	
	float dx = playerPosition.x - this->initialPosition.x;
	float dz = playerPosition.z - this->initialPosition.z;
	float angle = atan2(-dz, dx);

	glm::mat4 enemyModel =
		glm::translate(glm::mat4(1.0f), glm::vec3(this->initialPosition.x, this->initialPosition.y, this->initialPosition.z)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(angle*angleFactor), glm::vec3(0.0f, 1.0f, 0.0f))*
		glm::scale(glm::mat4(1.0f), glm::vec3(0.00008f, 0.00008f, 0.00008f));
	
	ModelMat(enemyModel);

	render(mainShader);
	renderEnemyWeapon(angle,mainShader);
	renderEnemyBullet(mainShader);
}

void Enemy::update(glm::vec3 playerPos){
	if (this->followPlayer){
		glm::vec3 direction = playerPos - this->initialPosition;
		this->initialPosition.x += direction.x*movSpeed;
		this->initialPosition.z += direction.z*movSpeed;
	}
	updateBulletsPosition();
}

void Enemy::updateBulletsPosition(){
	std::vector<Bullet*>::iterator bullet;
	for (bullet = weapon->CurrentBullets()->begin(); bullet != weapon->CurrentBullets()->end(); ++bullet) {

		try
		{
			(*bullet)->UpdatePosicion();
		}
		catch (const std::exception &exc)
		{
			printf(exc.what());
		}
	}
}


void Enemy::fire(glm::vec3 positionPlayer)
{
	glm::vec3 position = glm::vec3(this->initialPosition.x, this->initialPosition.y + pistolPosYDiff, this->initialPosition.z);
	glm::vec3 direction = positionPlayer - position;
	weapon->Fire(position, direction,0.008f);
}


