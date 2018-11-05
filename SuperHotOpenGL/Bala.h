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

using namespace std;




class Bala
{
private:
	
	glm::vec3 posicionActual;
	glm::vec3 velocidadActual;
	bool disparado;
	bool moviendose;
	bool recargado;
	
public:
	
	Bala();
	Bala(glm::vec3,glm::vec3,bool,bool,bool);
	glm::mat4 render();
			
};



