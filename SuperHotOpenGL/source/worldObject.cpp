#include <model.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <weapon.h>
#include <worldObject.h>

WorldObject::WorldObject(glm::mat4 initialModelMat, glm::vec3 specular, float specularRate)
{
	InitialModelMat(initialModelMat);
	Specular(specular);
	SpecularRate(specularRate);
	Mesh(NULL);
}

void WorldObject::freeResources()
{
	if (Mesh() != NULL)
	{
		// deleteModel	
	}
}

void WorldObject::render(Shader* shader)
{
	glm::mat3 mat_inv = glm::transpose(glm::inverse(glm::mat3(ModelMat())));
	shader->setMat3("m_3x3", mat_inv);
	shader->setVec3("mat_specular", Specular());
	shader->setFloat("mat_s", SpecularRate());

	shader->setMat4("model", ModelMat());	
	Mesh()->Draw(*shader);	
}