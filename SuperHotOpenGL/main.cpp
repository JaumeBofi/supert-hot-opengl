#include <iostream>
#include <cstdlib>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <settings.h>
#include <shader.h>
#include <model.h>

// Settings
Settings _settings = Settings();
Shader* _mainShader;
Model* model;

void display() {
	glClearColor(1.0, 0.0, 0.0, 0.1);
	glClear(GL_COLOR_BUFFER_BIT);

	// don't forget to enable shader before setting uniforms
	_mainShader->use();

	glutSwapBuffers();
}

bool init_resources()
{
	_mainShader = new Shader((_settings.ShadersDirectory() + "superhot-gl.vs").c_str(), (_settings.ShadersDirectory() + "superhot-gl.fs").c_str());
	model = new Model()
	return true;	
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitContextVersion(2, 0);
	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(_settings.ScrWidth(), _settings.ScrHeight());
	glutCreateWindow("SUPERHOT-GL");

	GLenum glew_status = glewInit();
	if (glew_status != GLEW_OK) {
		std::cout << "Error GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}

	if (!GLEW_VERSION_2_0) {
		std::cout << "No hay version 2.0" << std::endl;
		exit(EXIT_FAILURE);
	}

	init_resources();	
	glutDisplayFunc(display);

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glutMainLoop();

	return EXIT_SUCCESS;
}

