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

Enemy::Enemy() {
	this->initialPosition = glm::vec3(0, 0, 0);
	this->finalPosition = glm::vec3(0, 0, 0);
	
}
Enemy::Enemy(glm::vec3 initialPosition, glm::vec3 finalPosition) {
	this->initialPosition = initialPosition;
	this->finalPosition = finalPosition;
	
}

void Enemy::addBullet(glm::vec3 positionPlayer){
	glm::vec3 direction = positionPlayer - this->initialPosition;
	glm::vec3 position = glm::vec3(this->initialPosition.x, this->initialPosition.y+0.0065, this->initialPosition.z);
	Bala bala = Bala(this->initialPosition, glm::vec3(0, 0, 0), glm::normalize(direction), true, true, true);
	listaBalas.push_back(bala);
}

glm::mat4 Enemy::render(Model* modelPistol, Model* modelBullet, Shader* mainShader, float playerX, float playerZ) {
	
	float dx = this->initialPosition.x - playerX;
	float dz = this->initialPosition.z - playerZ;
	float ang = atan2(dz, dx);
	printf("ang\n");
	printf("ang\g");
	printf(" \nang: %f",ang);
	//printf("Enemy InitialPosition x: %f , Enemy FinalPosition y: %f", this->initialPosition, this->finalPosition);
	glm::mat4 enemyModel =
		//glm::rotate(glm::mat4(1.0f), glm::radians(ang-20.0f), glm::vec3(0.0f, 1.0f, 0.0f))*
		glm::translate(glm::mat4(1.0f), glm::vec3(this->initialPosition.x, this->initialPosition.y, this->initialPosition.z)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(0.00008f, 0.00008f, 0.00008f));


	renderPistol(modelPistol,modelBullet, mainShader);
	renderBullet(modelBullet, mainShader);
	return enemyModel;
}

void Enemy::renderPistol(Model* modelPistol,Model* modelBullet, Shader* mainShader){
	glm::mat4 modPistola =
		glm::translate(glm::mat4(1.0f), glm::vec3(this->initialPosition.x, this->initialPosition.y+0.0065, this->initialPosition.z)) * //llevar pistola junto a la posicion de la camara
		glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(-180.0f), glm::vec3(0.0f, 0.0f, 1.0f)) * //para que el modelo no aparezca de cabeza
		glm::scale(glm::mat4(1.0f), glm::vec3(0.00035f, 0.00035f, 0.00035f)) //para escalarlo a un tama�o realista
		;	// it's a bit too big for our scene, so scale it down
	glm::mat3 mat_inv_transpPistola = glm::transpose(glm::inverse(glm::mat3(modPistola)));
	mainShader->setMat3("m_3x3", mat_inv_transpPistola);
	mainShader->setVec3("mat_specular", glm::vec3(1.0, 1.0, 1.0));
	mainShader->setFloat("mat_s", 100);

	mainShader->setMat4("model", modPistola);
	modelPistol->Draw(*mainShader);

}

void Enemy::updateBulletsPosition(float increment){
	std::list<Bala>::iterator it;
	for (it = listaBalas.begin(); it != listaBalas.end(); ++it) {
		it->actualizarPosicion(increment*0.5);
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