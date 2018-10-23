#include <iostream>
#include <cstdlib>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <settings.h>
#include <shader.h>

// Settings
Settings _settings = Settings();
Shader* _mainShader;

void display() {
	glClearColor(1.0, 0.0, 0.0, 0.1);
	glClear(GL_COLOR_BUFFER_BIT);

	glutSwapBuffers();
}

bool init_resources()
{
	_mainShader = new Shader((_settings.ShadersDirectory() + "superhot-gl.vs").c_str(), (_settings.ShadersDirectory() + "superhot-gl.fs").c_str());
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

	

	glutDisplayFunc(display);
	glutMainLoop();

	return EXIT_SUCCESS;
}

