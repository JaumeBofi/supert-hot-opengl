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

using namespace std;




class Enemy
{
private:

	glm::vec3 initialPosition;
	glm::vec3 finalPosition;	
	


public:

	Enemy();
	Enemy(glm::vec3, glm::vec3);
	glm::mat4 initialRender();
	void moveEnemy(float increment);
	void shoot(glm::vec3 playerPosition);

};



