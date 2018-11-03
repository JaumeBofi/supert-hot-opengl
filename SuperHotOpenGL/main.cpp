#include <iostream>
#include <cstdlib>
#include <GL/glew.h>
#include <GL/freeglut.h>


#include <settings.h>
#include <shader.h>
#include <model.h>
#include <camera.h>

// Settings
Settings settings = Settings();
Shader* mainShader;
Model* model;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool prueba = false;

// Timing
int oldTimeSinceStart = 0;
float deltaTime = 0.01f;	// time between current frame and last frame

void display() {
	glClearColor(1.0, 0.0, 0.0, 0.1);
	glClear(GL_COLOR_BUFFER_BIT);

	// don't forget to enable shader before setting uniforms
	mainShader->use();
	int a = 4;

	glutSwapBuffers();
}

bool init_resources()
{
	printf("%s", (settings.ShadersDirectory() + "super_hot.vs").c_str());
	mainShader = new Shader((settings.ShadersDirectory() + "super_hot.vs").c_str(), (settings.ShadersDirectory() + "super_hot.fs").c_str());
	model = new Model("Arc170.obj");
	model->ComputeData();	
	return true;	
}

void onDisplay() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// don't forget to enable shader before setting uniforms
	mainShader->use();

	// view/projection transformations
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 200.0f);
	glm::mat4 view = camera.GetViewMatrix();
	mainShader->setMat4("projection", projection);
	mainShader->setMat4("view", view);
	mainShader->setVec3("cam_position", camera.Position);

	// render the loaded model
	glm::mat4 mod =				
		glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(model->scale, model->scale, model->scale)) *		
		glm::translate(glm::mat4(1.0f), glm::vec3(-model->centroid.x, -model->centroid.y, -model->centroid.z)); // translate it down so it's at the center of the scene
	;	// it's a bit too big for our scene, so scale it down
	glm::mat3 mat_inv_transp = glm::transpose(glm::inverse(glm::mat3(mod)));
	mainShader->setMat3("m_3x3", mat_inv_transp);
	mainShader->setVec3("mat_specular", glm::vec3(1.0, 1.0, 1.0));
	mainShader->setFloat("mat_s", 100);

	mainShader->setMat4("model", mod);
	model->Draw(*mainShader);

	glutSwapBuffers();
}

void mouseMovement(int xpos, int ypos) {

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);	
	glutPostRedisplay();
}

void keyboardInput(unsigned char keycode, int x, int y) {
	    if (keycode == 'q') {
		    prueba = !prueba;
	    }
		if (prueba) {
			printf("%d\n", deltaTime);
			switch (keycode)
			{
			case 's': // Escape key
				camera.ProcessKeyboardTest(BACKWARD, deltaTime);
				break;
			case 'w': // Escape key
				camera.ProcessKeyboardTest(FORWARD, deltaTime);
				break;
			case 'a': // Escape key
				camera.ProcessKeyboardTest(LEFT, deltaTime);
				break;
			case 'd': // Escape key
				camera.ProcessKeyboardTest(RIGHT, deltaTime);
				break;
			case 27: // Escape key			
				exit(EXIT_SUCCESS);
				break;
			}
			
		}
		else  {
			printf("%d\n", deltaTime);
			switch (keycode)
			{
			case 's': // Escape key
				camera.ProcessKeyboard(BACKWARD, deltaTime);
				break;
			case 'w': // Escape key
				camera.ProcessKeyboard(FORWARD, deltaTime);
				break;
			case 'a': // Escape key
				camera.ProcessKeyboard(LEFT, deltaTime);
				break;
			case 'd': // Escape key
				camera.ProcessKeyboard(RIGHT, deltaTime);
				break;
			case 27: // Escape key			
				exit(EXIT_SUCCESS);
				break;
			}
			
		}
	
		glutPostRedisplay();
	
	
	
	
	
	
}

void onIdle()
{
	/*int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);	
	deltaTime = timeSinceStart - oldTimeSinceStart;
	oldTimeSinceStart = timeSinceStart;*/
}


int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitContextVersion(2, 0);
	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(settings.ScrWidth(), settings.ScrHeight());
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

	//camera.SetLookPoint(glm::vec3(0.0f, 0.0f, 0.0f));
	glutDisplayFunc(onDisplay);	
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glutKeyboardFunc(keyboardInput);
	glutPassiveMotionFunc(mouseMovement);
	glutIdleFunc(onIdle);
	glutMainLoop();

	return EXIT_SUCCESS;
}

