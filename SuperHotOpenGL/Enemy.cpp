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

#include "Bala.h"
using namespace std;

const float angleFactor = 70.0f;
const float pistolPosYDiff = 0.0065f;
const float movSpeed = 0.002f;

Enemy::Enemy() {
	this->initialPosition = glm::vec3(0, 0, 0);
	this->finalPosition = glm::vec3(0, 0, 0);

}
Enemy::Enemy(glm::vec3 initialPosition, glm::vec3 finalPosition) {
	this->initialPosition = initialPosition;
	this->finalPosition = finalPosition;

}

Enemy::Enemy(glm::vec3 initialPosition, glm::vec3 finalPosition, bool followPlayer) {
	this->initialPosition = initialPosition;
	this->finalPosition = finalPosition;
	this->followPlayer = followPlayer;
}

void Enemy::addBullet(glm::vec3 positionPlayer){
	glm::vec3 position = glm::vec3(this->initialPosition.x, this->initialPosition.y + pistolPosYDiff, this->initialPosition.z);
	glm::vec3 direction = positionPlayer - position;

	Bala bala = Bala(position, glm::vec3(0, 0, 0), glm::normalize(direction), true, true, true);
	listaBalas.push_back(bala);
}

glm::mat4 Enemy::render(Model* modelPistol, Model* modelBullet, Shader* mainShader, float playerX, float playerZ) {

	float dx = playerX - this->initialPosition.x;
	float dz = playerZ - this->initialPosition.z;
	float angle = atan2(-dz, dx);
	printf("ang\n");
	printf("ang\g");
	printf(" \nang: %f", angle);
	//printf("Enemy InitialPosition x: %f , Enemy FinalPosition y: %f", this->initialPosition, this->finalPosition);
	glm::mat4 enemyModel =
		glm::translate(glm::mat4(1.0f), glm::vec3(this->initialPosition.x, this->initialPosition.y, this->initialPosition.z)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(angle*angleFactor), glm::vec3(0.0f, 1.0f, 0.0f))*
		glm::scale(glm::mat4(1.0f), glm::vec3(0.00008f, 0.00008f, 0.00008f));


	renderPistol(modelPistol, mainShader, angle);
	renderBullet(modelBullet, mainShader);
	return enemyModel;
}

void Enemy::renderPistol(Model* modelPistol, Shader* mainShader, float angle){
	glm::mat4 modPistola =
		glm::translate(glm::mat4(1.0f), glm::vec3(this->initialPosition.x, this->initialPosition.y + pistolPosYDiff, this->initialPosition.z)) * //llevar pistola junto a la posicion de la camara
		glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f + angle*angleFactor), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(-180.0f), glm::vec3(0.0f, 0.0f, 1.0f)) * //para que el modelo no aparezca de cabeza
		//glm::rotate(glm::mat4(1.0f), glm::radians(angle*angleFactor), glm::vec3(0.0f, 1.0f, 0.0f))*
		glm::scale(glm::mat4(1.0f), glm::vec3(0.00035f, 0.00035f, 0.00035f)) //para escalarlo a un tamaño realista
		;	// it's a bit too big for our scene, so scale it down
	glm::mat3 mat_inv_transpPistola = glm::transpose(glm::inverse(glm::mat3(modPistola)));
	mainShader->setMat3("m_3x3", mat_inv_transpPistola);
	mainShader->setVec3("mat_specular", glm::vec3(1.0, 1.0, 1.0));
	mainShader->setFloat("mat_s", 100);

	mainShader->setMat4("model", modPistola);
	modelPistol->Draw(*mainShader);

}

void Enemy::update(glm::vec3 playerPos){
	if (this->followPlayer){
		printf("follow");
		glm::vec3 direction = playerPos - this->initialPosition;
		this->initialPosition.x += direction.x*movSpeed;
		this->initialPosition.z += direction.z*movSpeed;

	}
	updateBulletsPosition();
}

void Enemy::updateBulletsPosition(){
	std::list<Bala>::iterator it;
	for (it = listaBalas.begin(); it != listaBalas.end(); ++it) {
		it->actualizarPosicion();
		//std::cout << it->;
	}
}

void Enemy::renderBullet(Model* modelBullet, Shader* mainShader){
	std::list<Bala>::iterator bala;
	for (bala = listaBalas.begin(); bala != listaBalas.end(); ++bala) {
		glm::mat4 modBullet = bala->render();
		/*lm::translate(glm::mat4(1.0f), glm::vec3(bala.this->initialPosition.x, this->initialPosition.y + 0.0065, this->initialPosition.z)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(0.00002f, 0.00002f, 0.00002f));*/
		glm::mat3 mat_inv_transpPistola = glm::transpose(glm::inverse(glm::mat3(modBullet)));
		mainShader->setMat3("m_3x3", mat_inv_transpPistola);
		mainShader->setVec3("mat_specular", glm::vec3(1.0, 1.0, 1.0));
		mainShader->setFloat("mat_s", 100);

		mainShader->setMat4("model", modBullet);
		modelBullet->Draw(*mainShader);
	}




}

//void Bala::actualizarPosicion(float increment) {
//	//this->posicionActual += this->direccion*0.5f;
//	this->posicionActual += this->direccion*increment;
//}