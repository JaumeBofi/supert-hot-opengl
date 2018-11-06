#include <model.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <weapon.h>

Weapon::Weapon(string modelName, int clipSize)
{
	Mesh(new Model(modelName));
	Mesh()->ComputeData();

	glm::mat4 initialModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.0025f, 0.0f))*  //para que la pistola baje a un nivel aceptable
		glm::rotate(glm::mat4(1.0f), glm::radians(-180.0f), glm::vec3(0.0f, 0.0f, 1.0f)) * //para que el modelo no aparezca de cabeza		
		glm::scale(glm::mat4(1.0f), glm::vec3(0.0005f, 0.0005f, 0.0005f)); //para escalarlo a un tamaño realista

	InitialModelMat(initialModel);
}

void Weapon::UpdatePosition(glm::mat4 model)
{
	ModelMat(model*InitialModelMat());
}