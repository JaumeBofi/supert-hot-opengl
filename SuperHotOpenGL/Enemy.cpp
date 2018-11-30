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

const float ANGLE_FACTOR = 70.0f;
const float PISTOL_POSY_DIFF = 0.0090f;
const float MOV_SPEED = 0.02f;
const float INCREMENT_FIRE_TIMER = 0.01f;
const float MAX_VALUE_TIMER = 1.0f;


Enemy::Enemy(){

}

Enemy::Enemy(string modelName,Weapon* weapon,glm::vec3 initialPosition) {
	Mesh(new Model(modelName));
	Mesh()->ComputeData();
	Mesh()->ComputeCollisionModel();
	this->initialPosition = initialPosition;
	this->finalPosition = finalPosition;
	this->followPlayer = false;
	this->weapon = weapon;
	glm::mat4 enemyModel =
		glm::translate(glm::mat4(1.0f), glm::vec3(this->initialPosition.x, this->initialPosition.y, this->initialPosition.z)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(0.00008f, 0.00008f, 0.00008f));

	InitialModelMat(enemyModel);

	this->fireTimer = 0.0f;
	this->dead = false;
}

Enemy::Enemy(string modelName,Weapon* weapon,glm::vec3 initialPosition,bool followPlayer) {
	Mesh(new Model(modelName));
	Mesh()->ComputeData();
	Mesh()->ComputeCollisionModel();
	this->initialPosition = initialPosition;
	this->finalPosition = finalPosition;
	this->followPlayer = followPlayer;
	this->weapon = weapon;
	glm::mat4 enemyModel =
		glm::translate(glm::mat4(1.0f), glm::vec3(this->initialPosition.x, this->initialPosition.y, this->initialPosition.z)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(0.00008f, 0.00008f, 0.00008f));

	InitialModelMat(enemyModel);
	this->fireTimer = 0.0f;
	this->dead = false;
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
		glm::translate(glm::mat4(1.0f), glm::vec3(this->initialPosition.x, this->initialPosition.y + PISTOL_POSY_DIFF, this->initialPosition.z)) * //llevar pistola junto a la posicion de la camara
		glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f + angle*ANGLE_FACTOR), glm::vec3(0.0f, 1.0f, 0.0f));  //con rotacion para seguir al jugador
	this->weapon->UpdatePosition(modWeapon);
	this->weapon->render(mainShader);
}

void Enemy::renderEnemy(glm::vec3 playerPosition, Shader* mainShader){
	
	float dx = playerPosition.x - this->initialPosition.x;
	float dz = playerPosition.z - this->initialPosition.z;
	float angle = atan2(-dz, dx);
	glm::mat4 enemyModel;
	if(this->dead) {
		 enemyModel =
			glm::translate(glm::mat4(1.0f), glm::vec3(this->initialPosition.x, this->initialPosition.y, this->initialPosition.z)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(angle*ANGLE_FACTOR), glm::vec3(0.0f, 1.0f, 0.0f))*
			 glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f))*
			glm::scale(glm::mat4(1.0f), glm::vec3(0.00008f, 0.00008f, 0.00008f));
	}
	else {
		enemyModel =
			glm::translate(glm::mat4(1.0f), glm::vec3(this->initialPosition.x, this->initialPosition.y, this->initialPosition.z)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(angle*ANGLE_FACTOR), glm::vec3(0.0f, 1.0f, 0.0f))*
			glm::scale(glm::mat4(1.0f), glm::vec3(0.00008f, 0.00008f, 0.00008f));

		
	}
	
	ModelMat(enemyModel);
	Mesh()->UpdateCollisionModel(enemyModel);

	render(mainShader);
	renderEnemyBullet(mainShader);


	if (this->dead) {
		//
	}
	else {
		renderEnemyWeapon(angle, mainShader);
	}
}

void Enemy::update(glm::vec3 playerPos){

	if (this->followPlayer && !this->dead){
		glm::vec3 direction = playerPos - this->initialPosition;
		this->initialPosition.x += direction.x*MOV_SPEED;
		this->initialPosition.z += direction.z*MOV_SPEED;
	}
	
	this->fireTimer += INCREMENT_FIRE_TIMER;
	if (this->fireTimer >= MAX_VALUE_TIMER){
		this->fireTimer = 0.0f;
		fire(playerPos);
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
	glm::vec3 position = glm::vec3(this->initialPosition.x, this->initialPosition.y + PISTOL_POSY_DIFF, this->initialPosition.z);
	glm::vec3 direction = positionPlayer - position;
	weapon->Fire(position, direction,0.008f);
}


Weapon* Enemy::CurrentWeapon() {
	return this->weapon;
}

bool Enemy::isDead() {
	return this->dead;
}
void Enemy::setDead() {
	this->dead = true;
}

