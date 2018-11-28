#pragma once
#include <model.h>
#include <glm/glm.hpp>

 class WorldObject
{
private:
	Model* _mesh;	
	glm::mat4 _modelMat;
	glm::mat4 _initialModelMat;
	glm::vec3 _specular;
	float _specularRate;
	

public:

	WorldObject(glm::mat4 initialModelMat = glm::mat4(), glm::vec3 specular = glm::vec3(0.0f, 0.0f, 0.0f), float specularRate = 100.0f);
	void freeResources();
	void render(Shader* shader);

	Model* Mesh() const { return _mesh; }
	void Mesh(Model* val) { _mesh = val; }
	glm::mat4 ModelMat() const { return _modelMat; }
	void ModelMat(glm::mat4 val) { _modelMat = val; }
	glm::vec3 Specular() const { return _specular; }
	void Specular(glm::vec3 val) { _specular = val; }
	float SpecularRate() const { return _specularRate; }
	void SpecularRate(float val) { _specularRate = val; }
	glm::mat4 InitialModelMat() const { return _initialModelMat; }
	void InitialModelMat(glm::mat4 val) { _initialModelMat = val; }
 };