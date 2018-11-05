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
#include "Bala.h"

using namespace std;

Bala::Bala() {
	this->posicionActual = glm::vec3(0, 0, 0);
	this->velocidadActual = glm::vec3(0, 0, 0);
	this->disparado = false;
	this->moviendose = false;
	this->recargado = false;
}
Bala::Bala(glm::vec3 posicionActual, glm::vec3 velocidadActual, bool disparado, bool moviendose, bool recargado) {
	this->posicionActual = posicionActual;
	this->velocidadActual = velocidadActual;
	this->disparado = disparado;
	this->moviendose = moviendose;
	this->recargado = recargado;
}
glm::mat4 Bala::render() {
	glm::mat4 modBala =
		glm::translate(glm::mat4(1.0f), glm::vec3(this->posicionActual.x+0.05, this->posicionActual.y, this->posicionActual.z)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(0.0005f, 0.0005f, 0.0005f))
		;	// it's a bit too big for our scene, so scale it down

	return modBala;
}